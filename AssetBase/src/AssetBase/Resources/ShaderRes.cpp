#include "pch.h"
#include "ShaderRes.h"

#include "AssetBase/AssetBase.h"
#include "AssetBase/Util/OptionalField.h"

#include "ShaderConductor/ShaderConductor.hpp"
#include <d3dcompiler.h>
#include <wrl/client.h>

#include <stdint.h>
#include <filesystem>
#include <sstream>

#undef max

namespace fs = std::filesystem;
namespace fb = flatbuffers;
namespace SC = ShaderConductor;
using namespace Microsoft::WRL;

struct ShaderStore
{
	fb::Offset<fb::String> GL_Vert;
	fb::Offset<fb::String> GL_Frag;
	fb::Offset<fb::String> GL_Comp;

	fb::Offset<fb::Vector<uint8_t>> D3D_VS;
	fb::Offset<fb::Vector<uint8_t>> D3D_PS;
	fb::Offset<fb::Vector<uint8_t>> D3D_CS;
};

fs::path ResolveInclude(const char* path)
{
	if (fs::exists(path))
		return path;

	for (const std::string& include_path : AssetBase::Get()->GetIncludePaths())
	{
		fs::path fs_path = fs::path(include_path) / fs::path(path).filename();
		if (fs::exists(fs_path))
			return fs_path;
	}

	return "";
}

class ShaderInclude : public ID3DInclude
{
public:
	HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
	{
		fs::path path = ResolveInclude(pFileName);
		if (path == "")
			return E_FAIL;

		if (IncludeType != D3D_INCLUDE_LOCAL)
			return E_FAIL;

		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.is_open())
			return E_FAIL;

		file.ignore(std::numeric_limits<std::streamsize>::max());
		size_t file_size = file.gcount();
		*pBytes = static_cast<UINT>(file_size);
		file.clear();
		file.seekg(0, std::ios_base::beg);

		char* data = reinterpret_cast<char*>(std::malloc(file_size));

		file.read(data, file_size);
		file.close();

		*ppData = data;
		return S_OK;
	}

	HRESULT Close(LPCVOID pData) override
	{
		std::free(const_cast<void*>(pData));
		return S_OK;
	}
};

SC::Blob LoadInclude(const char* include_name)
{
	fs::path path = ResolveInclude(include_name);
	if (path == "")
		throw std::runtime_error("Include file does not exist: " + std::string(include_name));

	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Failed to open include file " + std::string(include_name));

	file.ignore(std::numeric_limits<std::streamsize>::max());
	size_t file_size = file.gcount();
	file.clear();
	file.seekg(0, std::ios_base::beg);

	char* data = reinterpret_cast<char*>(std::malloc(file_size));

	file.read(data, file_size);
	file.close();

	SC::Blob blob(data, static_cast<uint32_t>(file_size));

	std::free(data);
	return blob;
}

static void CompileHLSL(fb::FlatBufferBuilder& builder, ShaderStore& store, const fs::path& entry_path, bool debug_mode);
static std::string GetStageCharacters(const std::string& filename);
fb::Offset<fb::Vector<uint8_t>> CompileD3D11(fb::FlatBufferBuilder& builder, const fs::path& entry_path, const std::string& entrypoint, const std::string& target, bool debug_mode);
static fb::Offset<fb::Vector<fb::Offset<Assets::ShaderSampler>>> LoadSamplers(fb::FlatBufferBuilder& builder, const YAML::Node& shader);

fb::Offset<Assets::Shader> SerializeShader(
	fb::FlatBufferBuilder& builder,
	const std::filesystem::path& base_path,
	const std::string& name, YAML::Node shader, bool debug_mode)
{
	ShaderStore store;
	fs::path shader_path{ GetOptionalString(shader, "path") };

	// SHADER SOURCES //
	for (const fs::directory_entry& entry : fs::directory_iterator(shader_path))
	{
		const fs::path& entry_path = entry.path();
		if (!fs::is_regular_file(entry_path))
			continue;

		if (entry_path.extension() == ".hlsl")
			CompileHLSL(builder, store, entry_path, debug_mode);
	}

	auto samplers_offset = LoadSamplers(builder, shader);

	return Assets::CreateShader(builder, builder.CreateString(name),
		store.GL_Vert, store.GL_Frag, store.GL_Comp,
		store.D3D_VS, store.D3D_PS, store.D3D_CS, samplers_offset);
}

static void CompileHLSL(fb::FlatBufferBuilder& builder, ShaderStore& store, const fs::path& entry_path, bool debug_mode)
{
	constexpr int TargetCount = 1;
	constexpr int GlslIndex = 0;
	//constexpr int SpirvIndex = 1;

	std::ifstream shader_file(entry_path);
	std::string hlsl_source{ std::istreambuf_iterator<char>(shader_file), std::istreambuf_iterator<char>() };

	SC::Compiler::TargetDesc targets[TargetCount];
	SC::Compiler::ResultDesc results[TargetCount];

	SC::Compiler::SourceDesc source;
	std::string path = entry_path.u8string();
	source.source = hlsl_source.c_str();
	source.fileName = path.c_str();
	source.defines = nullptr;
	source.numDefines = 0;
	source.loadIncludeCallback = LoadInclude;

	SC::Compiler::Options options;
	options.enableDebugInfo = debug_mode;
	options.disableOptimizations = !debug_mode;
	options.optimizationLevel = debug_mode ? 0 : 3;
	options.shaderModel.major_ver = 5;
	options.shaderModel.minor_ver = 0;
	options.packMatricesInRowMajor = false;

	SC::Compiler::TargetDesc& target_glsl = targets[GlslIndex];
	target_glsl.language = SC::ShadingLanguage::Glsl;
	target_glsl.version = "430";
	target_glsl.asModule = false;

	//SC::Compiler::TargetDesc& target_spirv = targets[SpirvIndex];
	//target_spirv.language = SC::ShadingLanguage::SpirV;
	//target_spirv.version = "1.3";
	//target_spirv.asModule = false;

	std::string filename = entry_path.filename().string();
	std::string stage_chars = GetStageCharacters(filename);
	for (char stage_char : stage_chars)
	{
		std::string entrypoint = "?s_main";
		entrypoint[0] = stage_char;
		source.entryPoint = entrypoint.c_str();

		std::string target = "?s_5_0";
		target[0] = stage_char;

		fb::Offset<fb::Vector<uint8_t>>* dxil_offset = nullptr;
		fb::Offset<fb::String>* glsl_offset = nullptr;
		switch (stage_char)
		{
		case 'v':
			dxil_offset = &store.D3D_VS;
			glsl_offset = &store.GL_Vert;
			source.stage = SC::ShaderStage::VertexShader;
			break;
		case 'p':
			dxil_offset = &store.D3D_PS;
			glsl_offset = &store.GL_Frag;
			source.stage = SC::ShaderStage::PixelShader;
			break;
		case 'c':
			dxil_offset = &store.D3D_CS;
			glsl_offset = &store.GL_Comp;
			source.stage = SC::ShaderStage::ComputeShader;
			break;
		default:
			throw std::runtime_error(std::string("Unknown shader type character ") + stage_char);
		}

		if (dxil_offset->o)
			throw std::runtime_error(filename + ": invalid HLSL shader filename: character '" + stage_char + "' appears more than once.");

		*dxil_offset = CompileD3D11(builder, entry_path, entrypoint, target, debug_mode);

		// SHADER CONDUCTOR //
		SC::Compiler::Compile(source, options, targets, TargetCount, results);

		for (int i = 0; i < TargetCount; i++)
		{
			if (results[i].hasError)
			{
				std::string error_msg((const char*)results[i].errorWarningMsg.Data(), results[i].errorWarningMsg.Size());
				throw std::runtime_error(error_msg);
			}
		}

		// GLSL Source code.
		const SC::Compiler::ResultDesc& glsl_result = results[GlslIndex];
		*glsl_offset = builder.CreateString((const char*)glsl_result.target.Data(), glsl_result.target.Size());

		//std::cout.write((const char*)glsl_result.target.Data(), glsl_result.target.Size());

		// TODO: SPIR-V Bytecode.
	}
}

std::string GetStageCharacters(const std::string& filename)
{
	size_t underscore_pos = filename.rfind('_');
	if (underscore_pos == std::string::npos)
		throw std::runtime_error(filename + ": invalid HLSL shader filename: missing '_'");

	size_t dot_pos = filename.rfind('.');
	if (dot_pos == std::string::npos)
		throw std::runtime_error(filename + ": invalid HLSL shader filename: missing '.'");

	if (dot_pos - underscore_pos <= 2)
		throw std::runtime_error(filename + ": invalid HLSL shader filename: incorrect positioning of '_' and '.'");

	return filename.substr(underscore_pos + 1, dot_pos - underscore_pos - 2);
}

fb::Offset<fb::Vector<uint8_t>> CompileD3D11(fb::FlatBufferBuilder& builder, const fs::path& entry_path,
	const std::string& entrypoint, const std::string& target, bool debug_mode)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	if (debug_mode)
		flags |= D3DCOMPILE_DEBUG;
	else
		flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

	ShaderInclude include_handler;
	ComPtr<ID3D10Blob> dxil_shader;
	ComPtr<ID3D10Blob> error_message;
	HRESULT result = D3DCompileFromFile(entry_path.c_str(), NULL, &include_handler, entrypoint.c_str(), target.c_str(), flags, 0, &dxil_shader, &error_message);

	if (FAILED(result))
		throw std::runtime_error(std::string{ (const char*)error_message->GetBufferPointer(), error_message->GetBufferSize() });

	uint8_t* dxil_vector_data = nullptr;
	fb::Offset<fb::Vector<uint8_t>> dxil_offset = builder.CreateUninitializedVector(dxil_shader->GetBufferSize(), &dxil_vector_data);
	memcpy(dxil_vector_data, dxil_shader->GetBufferPointer(), dxil_shader->GetBufferSize());

	return dxil_offset;
}

fb::Offset<fb::Vector<fb::Offset<Assets::ShaderSampler>>>
	LoadSamplers(fb::FlatBufferBuilder& builder, const YAML::Node& shader)
{
	std::vector<fb::Offset<Assets::ShaderSampler>> sampler_offsets;
	YAML::Node samplers = shader["samplers"];
	for (const std::pair<YAML::Node, YAML::Node>& sampler : samplers)
	{
		int binding = sampler.first.as<int>();
		if (binding < 0 || binding > UINT8_MAX)
		{
			throw std::runtime_error((std::stringstream() <<
				"Sampler binding " << binding << " is out of bounds for uint8").str());
		}
		if (!sampler.second.IsSequence())
		{
			throw std::runtime_error((std::stringstream() <<
				"Sampler with binding " << binding << " is not a sequence").str());
		}

		constexpr int NumSamplerStrings = 2;
		if (sampler.second.size() != NumSamplerStrings)
		{
			throw std::runtime_error((std::stringstream() <<
				"Sampler with binding " << binding << " does not have 2 entries").str());
		}

		std::string combined = "SPIRV_Cross_Combined";
		for (int i = 0; i < NumSamplerStrings; i++)
		{
			YAML::Node name = sampler.second[i];
			if (!name.IsScalar())
			{
				std::stringstream ss;
				ss << "Sampler with binding " << binding << " is not a string";
				throw std::runtime_error(ss.str());
			}
			combined += name.Scalar();
		}

		fb::Offset<fb::String> combined_offset = builder.CreateString(combined);
		sampler_offsets.push_back(Assets::CreateShaderSampler(builder, binding, combined_offset));
	}
	return builder.CreateVector(sampler_offsets);
}
