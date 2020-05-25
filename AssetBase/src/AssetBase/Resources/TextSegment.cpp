#include "pch.h"
#include "TextSegment.h"

namespace AssetBase
{
	TextSegment::TextSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath)
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

		std::cout << "Loading text '" << name << "' ... ";

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
		fileSize = shaderFile.gcount();
		shaderFile.clear();
		shaderFile.seekg(0, std::ios_base::beg);

		textData = new char[fileSize];

		shaderFile.read((char*)&textData[0], fileSize);
		shaderFile.close();

		std::cout << "done.\n";
	}

	TextSegment::~TextSegment()
	{
		delete[] textData;
	}

	size_t TextSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + sizeof(fileSize) + fileSize;
	}

	std::ostream& operator<<(std::ostream& os, const TextSegment& t)
	{
		os.write((const char*)&t.type, sizeof(t.type));
		os.write(t.name, sizeof(t.name));
		os.write((char*)&t.fileSize, sizeof(t.fileSize));
		os.write(t.textData, t.fileSize);
		return os;
	}
}
