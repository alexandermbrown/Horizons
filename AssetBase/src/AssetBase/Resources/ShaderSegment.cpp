#include "pch.h"
#include "ShaderSegment.h"

namespace AssetBase
{
	ShaderSegment::ShaderSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath)
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

		std::filesystem::path shaderPath;
		for (rapidxml::xml_node<>* node = shaderNode->first_node(); node; node = node->next_sibling())
		{
			if (!strcmp(node->name(), "source"))
				shaderPath = node->value();
		}

		if (shaderPath.empty())
			throw "Missing <source> in shader.\n";

		std::ifstream shaderFile(basePath.parent_path() / shaderPath, std::ios::in | std::ios::binary);
		if (!shaderFile.is_open()) {
			throw "Error opening shader source file.\n";
		}

		shaderFile.ignore(std::numeric_limits<std::streamsize>::max());
		shaderSize = shaderFile.gcount();
		shaderFile.clear();
		shaderFile.seekg(0, std::ios_base::beg);

		shaderData = new char[shaderSize];

		shaderFile.read((char*)&shaderData[0], shaderSize);
		shaderFile.close();

		std::cout << "done.\n";
	}

	ShaderSegment::~ShaderSegment()
	{
		delete[] shaderData;
	}

	size_t ShaderSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + sizeof(shaderSize) + shaderSize;
	}

	std::ostream& operator<<(std::ostream& os, const ShaderSegment& s)
	{
		os.write((const char*)&s.type, sizeof(s.type));
		os.write(s.name, sizeof(s.name));
		os.write((char*)&s.shaderSize, sizeof(s.shaderSize));
		os.write(s.shaderData, s.shaderSize);
		return os;
	}
}
