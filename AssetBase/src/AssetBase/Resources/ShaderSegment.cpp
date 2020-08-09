#include "pch.h"
#include "ShaderSegment.h"

#include <d3dcompiler.h>

#undef max

namespace AssetBase
{
	ShaderSegment::ShaderSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath, bool debugMode)
		: Segment(SegmentType::Shader)
	{
		name[0] = '\0';
		for (rapidxml::xml_attribute<>* attr = shaderNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name")) {
				strcpy_s(name, attr->value());
				break;
			}
		}
		if (strlen(name) == 0)
			throw "Attribute 'name' not found in shader.\n";

		std::cout << "Loading shader '" << name << "' ... ";

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

		// OPENGL
		{
			std::ifstream glslFile(basePath.parent_path() / glslPath, std::ios::in | std::ios::binary);
			if (!glslFile.is_open()) {
				throw "Error opening glsl shader source file.\n";
			}

			glslFile.ignore(std::numeric_limits<std::streamsize>::max());
			glslSize = glslFile.gcount();
			glslFile.clear();
			glslFile.seekg(0, std::ios_base::beg);

			glslData = new char[glslSize];

			glslFile.read((char*)&glslData[0], glslSize);
			glslFile.close();
		}

		// DIRECT3D 11 SHADER COMPILATION.
		{
			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
			if (debugMode) {
				flags |= D3DCOMPILE_DEBUG;
			}
			else {
				flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
			}

			ID3D10Blob* vertexShaderBuffer = nullptr;
			ID3D10Blob* pixelShaderBuffer = nullptr;

			ID3D10Blob* errorMessage = nullptr;

			HRESULT result = D3DCompileFromFile(hlslPath.c_str(), NULL, NULL, "vs_main", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage);
			if (FAILED(result))
			{
				std::cout.write((const char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
				throw "HLSL vertex shader compilation error.";
			}

			result = D3DCompileFromFile(hlslPath.c_str(), NULL, NULL, "ps_main", "ps_5_0", flags, 0, &pixelShaderBuffer, &errorMessage);

			if (FAILED(result))
			{
				std::cout.write((const char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
				throw "HLSL pixel shader compilation error.";
			}

			hlslVsSize = vertexShaderBuffer->GetBufferSize();
			hlslVsData = new char[hlslVsSize];
			memcpy(hlslVsData, vertexShaderBuffer->GetBufferPointer(), hlslVsSize);

			hlslPsSize = pixelShaderBuffer->GetBufferSize();
			hlslPsData = new char[hlslPsSize];
			memcpy(hlslPsData, pixelShaderBuffer->GetBufferPointer(), hlslPsSize);

			vertexShaderBuffer->Release();
			pixelShaderBuffer->Release();
		}
		
		std::cout << "done.\n";
	}

	ShaderSegment::~ShaderSegment()
	{
		delete[] glslData;
	}

	size_t ShaderSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + sizeof(glslSize) + glslSize
			+ sizeof(hlslVsSize) + hlslVsSize + sizeof(hlslPsSize) + hlslPsSize;
	}

	std::ostream& operator<<(std::ostream& os, const ShaderSegment& s)
	{
		os.write((const char*)&s.type, sizeof(s.type));
		os.write(s.name, sizeof(s.name));

		os.write((char*)&s.glslSize, sizeof(s.glslSize));
		os.write(s.glslData, s.glslSize);

		os.write((char*)&s.hlslVsSize, sizeof(s.hlslVsSize));
		os.write(s.hlslVsData, s.hlslVsSize);

		os.write((char*)&s.hlslPsSize, sizeof(s.hlslPsSize));
		os.write(s.hlslPsData, s.hlslPsSize);

		return os;
	}
}
