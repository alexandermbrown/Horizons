#include "pch.h"
#include "ShaderSegment.h"

#include "OptionalField.h"

#include <d3dcompiler.h>
#include <stdint.h>
#include <filesystem>

namespace AssetBase
{
	flatbuffers::Offset<Assets::Shader> SerializeShader(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node shader, bool debug_mode)
	{
		flatbuffers::Offset<flatbuffers::String> name_offset = builder.CreateString(name);
		
		std::filesystem::path glsl_path(GetOptionalString(shader, "glsl"));
		std::filesystem::path hlsl_path(GetOptionalString(shader, "hlsl"));

		// --------
		//  OPENGL
		// --------
		std::ifstream glsl_file(base_path.parent_path() / glsl_path, std::ios::in | std::ios::binary);
		if (!glsl_file.is_open())
			throw std::runtime_error("Error opening glsl shader source file " + (base_path.parent_path() / glsl_path).string());

		std::stringstream glsl_stream;
		glsl_stream << glsl_file.rdbuf();
		auto glsl = builder.CreateString(glsl_stream.str());

		// -------------
		//  DIRECT3D 11
		// -------------
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		if (debug_mode)
			flags |= D3DCOMPILE_DEBUG;
		else
			flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

		ID3D10Blob* vs_buffer = nullptr;
		ID3D10Blob* ps_buffer = nullptr;
		ID3D10Blob* error_message = nullptr;

		std::filesystem::path actual_path = base_path.parent_path() / hlsl_path;

		HRESULT result = D3DCompileFromFile(actual_path.c_str(), NULL, NULL, "vs_main", "vs_5_0", flags, 0, &vs_buffer, &error_message);
		if (FAILED(result))
		{
			std::cout.write((const char*)error_message->GetBufferPointer(), error_message->GetBufferSize());
			throw "HLSL vertex shader compilation error.";
		}

		result = D3DCompileFromFile(actual_path.c_str(), NULL, NULL, "ps_main", "ps_5_0", flags, 0, &ps_buffer, &error_message);
		if (FAILED(result))
		{
			std::cout.write((const char*)error_message->GetBufferPointer(), error_message->GetBufferSize());
			throw "HLSL pixel shader compilation error.";
		}

		uint8_t* vs_data = nullptr;
		auto hlsl_vs = builder.CreateUninitializedVector(vs_buffer->GetBufferSize(), &vs_data);
		memcpy(vs_data, vs_buffer->GetBufferPointer(), vs_buffer->GetBufferSize());
		vs_buffer->Release();

		uint8_t* ps_data = nullptr;
		auto hlsl_ps = builder.CreateUninitializedVector(ps_buffer->GetBufferSize(), &ps_data);
		memcpy(ps_data, ps_buffer->GetBufferPointer(), ps_buffer->GetBufferSize());
		ps_buffer->Release();
		
		return Assets::CreateShader(builder, name_offset, glsl, hlsl_vs, hlsl_ps);
	}
}
