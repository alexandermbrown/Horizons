#include "pch.h"
#include "ShaderSegment.h"

#include "OptionalField.h"

#include <d3dcompiler.h>
#include <wrl/client.h>

#include <stdint.h>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;
using namespace Microsoft::WRL;

namespace AssetBase
{
	struct ShaderStore
	{
		flatbuffers::Offset<flatbuffers::String> GL_Vert;
		flatbuffers::Offset<flatbuffers::String> GL_Frag;
		flatbuffers::Offset<flatbuffers::String> GL_Comp;

		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> D3D_VS;
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> D3D_PS;
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> D3D_CS;
	};

	static void LoadGLShader(flatbuffers::FlatBufferBuilder& builder,
		const fs::path& shader_path, flatbuffers::Offset<flatbuffers::String>& dest);

	static void LoadD3DShader(flatbuffers::FlatBufferBuilder& builder, ShaderStore& store,
		const fs::path& entry_path, UINT flags);

	flatbuffers::Offset<Assets::Shader> SerializeShader(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node shader, bool debug_mode)
	{
		ShaderStore store;
		fs::path shader_path{ GetOptionalString(shader, "path") };

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		if (debug_mode)
			flags |= D3DCOMPILE_DEBUG;
		else
			flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

		for (const fs::directory_entry& entry : fs::directory_iterator(shader_path))
		{
			const fs::path& entry_path = entry.path();
			if (!fs::is_regular_file(entry_path))
				continue;

			fs::path extension = entry_path.extension();
			if (extension == ".vert")
				LoadGLShader(builder, entry_path, store.GL_Vert);
			else if (extension == ".frag")
				LoadGLShader(builder, entry_path, store.GL_Frag);
			else if (extension == ".comp")
				LoadGLShader(builder, entry_path, store.GL_Comp);
			else if (extension == ".hlsl")
				LoadD3DShader(builder, store, entry_path, flags);
		}
		
		return Assets::CreateShader(builder, builder.CreateString(name),
			store.GL_Vert, store.GL_Frag, store.GL_Comp,
			store.D3D_VS, store.D3D_PS, store.D3D_CS);
	}

	static void LoadGLShader(flatbuffers::FlatBufferBuilder& builder,
		const fs::path& shader_path, flatbuffers::Offset<flatbuffers::String>& dest)
	{
		std::ifstream shader_file(shader_path);
		if (!shader_file.is_open())
			throw std::runtime_error("Error opening glsl shader source file " + shader_path.string());
		std::string glsl_source{ std::istreambuf_iterator<char>(shader_file), std::istreambuf_iterator<char>() };
		dest = builder.CreateString(glsl_source);
	}

	static void LoadD3DShader(flatbuffers::FlatBufferBuilder& builder, ShaderStore& store, const fs::path& entry_path, UINT flags)
	{
		std::string filename = entry_path.filename().string();

		size_t underscore_pos = filename.rfind('_');
		if (underscore_pos == std::string::npos)
			throw std::runtime_error(filename + ": invalid HLSL shader filename: missing '_'");

		size_t dot_pos = filename.rfind('.');
		if (dot_pos == std::string::npos)
			throw std::runtime_error(filename + ": invalid HLSL shader filename: missing '.'");

		if (dot_pos - underscore_pos <= 2)
			throw std::runtime_error(filename + ": invalid HLSL shader filename: incorrect positioning of '_' and '.'");

		std::string type = filename.substr(underscore_pos + 1, dot_pos - underscore_pos - 2);

		for (char shader_char : type)
		{
			flatbuffers::Offset<flatbuffers::Vector<uint8_t>>* shader_offset = nullptr;
			switch (shader_char)
			{
			case 'v':
				shader_offset = &store.D3D_VS;
				break;
			case 'p':
				shader_offset = &store.D3D_PS;
				break;
			case 'c':
				shader_offset = &store.D3D_CS;
				break;
			default:
				throw std::runtime_error(std::string("Unknown shader type character ") + shader_char);
			}

			if (shader_offset->o)
				throw std::runtime_error(filename + ": invalid HLSL shader filename: character '" + shader_char + "' appears more than once.");

			ComPtr<ID3D10Blob> compiled_shader;
			ComPtr<ID3D10Blob> error_message;

			std::string entrypoint = "?s_main";
			std::string target = "?s_5_0";
			entrypoint[0] = shader_char;
			target[0] = shader_char;

			HRESULT result = D3DCompileFromFile(entry_path.c_str(), NULL, NULL, entrypoint.c_str(), target.c_str(), flags, 0, &compiled_shader, &error_message);

			if (FAILED(result))
				throw std::runtime_error(std::string{ (const char*)error_message->GetBufferPointer(), error_message->GetBufferSize() });

			uint8_t* vector_data = nullptr;
			*shader_offset = builder.CreateUninitializedVector(compiled_shader->GetBufferSize(), &vector_data);
			memcpy(vector_data, compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize());
		}
	}
}
