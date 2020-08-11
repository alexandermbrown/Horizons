#include "pch.h"
#include "ShaderSegment.h"

#include <d3dcompiler.h>
#include <stdint.h>
#include <filesystem>

namespace AssetBase
{
	flatbuffers::Offset<Assets::Shader> ShaderSegment::Serialize(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode)
	{
		flatbuffers::Offset<flatbuffers::String> name = NULL;
		for (rapidxml::xml_attribute<>* attr = shaderNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name")) {
				name = builder.CreateString(attr->value());
				std::cout << "Loading shader '" << attr->value() << "' ... ";
				break;
			}
		}
		if (name.IsNull())
			throw "Attribute 'name' not found in shader.\n";
		

		std::filesystem::path glslPath;
		std::filesystem::path hlslPath;
		for (rapidxml::xml_node<>* node = shaderNode->first_node(); node; node = node->next_sibling())
		{
			if (!strcmp(node->name(), "glsl"))
				glslPath = node->value();
			else if (!strcmp(node->name(), "hlsl"))
				hlslPath = node->value();
		}

		if (glslPath.empty())
			throw "Missing <glsl> in shader.\n";
		if (hlslPath.empty())
			throw "Missing <hlsl> in shader.\n";

		// -----
		// OPENGL
		// -----
		std::ifstream glslFile(basePath.parent_path() / glslPath, std::ios::in | std::ios::binary);
		if (!glslFile.is_open())
			throw "Error opening glsl shader source file.\n";

		std::stringstream glslStream;
		glslStream << glslFile.rdbuf();
		auto glsl = builder.CreateString(glslStream.str());

		// -----------
		// DIRECT3D 11
		// -----------
		
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		if (debugMode) {
			flags |= D3DCOMPILE_DEBUG;
		}
		else {
			flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
		}

		ID3D10Blob* vsBuffer = nullptr;
		ID3D10Blob* psBuffer = nullptr;

		ID3D10Blob* errorMessage = nullptr;

		HRESULT result = D3DCompileFromFile(hlslPath.c_str(), NULL, NULL, "vs_main", "vs_5_0", flags, 0, &vsBuffer, &errorMessage);
		if (FAILED(result))
		{
			std::cout.write((const char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
			throw "HLSL vertex shader compilation error.";
		}

		result = D3DCompileFromFile(hlslPath.c_str(), NULL, NULL, "ps_main", "ps_5_0", flags, 0, &psBuffer, &errorMessage);

		if (FAILED(result))
		{
			std::cout.write((const char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
			throw "HLSL pixel shader compilation error.";
		}

		uint8_t* vsData;
		auto hlsl_vs = builder.CreateUninitializedVector(vsBuffer->GetBufferSize(), &vsData);
		memcpy(vsData, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize());
		vsBuffer->Release();

		uint8_t* psData;
		auto hlsl_ps = builder.CreateUninitializedVector(psBuffer->GetBufferSize(), &psData);
		memcpy(psData, psBuffer->GetBufferPointer(), psBuffer->GetBufferSize());
		psBuffer->Release();
		
		std::cout << "done.\n";
		return Assets::CreateShader(builder, name, glsl, hlsl_vs, hlsl_ps);
	}
}
