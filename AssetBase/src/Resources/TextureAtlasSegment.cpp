#include "TextureAtlasSegment.h"

#include <string>
#include "Helpers.h"
#include "glm/gtc/type_ptr.hpp"

namespace AssetBase
{
	TextureAtlasSegment::TextureAtlasSegment()
		: Segment(SegmentType::TextureAtlas), entries()
	{

	}

	TextureAtlasSegment::TextureAtlasSegment(rapidxml::xml_node<>* atlasNode, const std::filesystem::path& basePath)
		: Segment(SegmentType::TextureAtlas), entries()
	{
		for (rapidxml::xml_attribute<>* attr = atlasNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name"))
			{
				strcpy_s(name, attr->value());
				break;
			}
		}
		if (name == "")
			throw "Attribute 'name' not found in shader.";

		for (rapidxml::xml_node<>* innerNode = atlasNode->first_node(); innerNode; innerNode = innerNode->next_sibling())
		{
			if (!strcmp(innerNode->name(), "texture"))
			{
				strcpy_s(texture, innerNode->value());
			}
			else if (!strcmp(innerNode->name(), "entries"))
			{
				for (rapidxml::xml_node<>* entryNode = innerNode->first_node(); entryNode; entryNode = entryNode->next_sibling())
				{
					AtlasEntry entry;
					for (rapidxml::xml_attribute<>* entryAttr = entryNode->first_attribute(); entryAttr; entryAttr = entryAttr->next_attribute())
					{
						if (!strcmp(entryAttr->name(), "alias"))
							strcpy_s(entry.alias, entryAttr->value());
						else if (!strcmp(entryAttr->name(), "x"))
							entry.bounds.x = Helpers::StringToFloat(entryAttr->value(), "entry.x");
						else if (!strcmp(entryAttr->name(), "y"))
							entry.bounds.y = Helpers::StringToFloat(entryAttr->value(), "entry.y");
						else if (!strcmp(entryAttr->name(), "width"))
							entry.bounds.z = Helpers::StringToFloat(entryAttr->value(), "entry.width");
						else if (!strcmp(entryAttr->name(), "height"))
							entry.bounds.w = Helpers::StringToFloat(entryAttr->value(), "entry.height");
					}

					entries.push_back(entry);
				}
			}
		}
	}

	size_t TextureAtlasSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) +  sizeof(texture) + sizeof(uint32_t) + AtlasEntry::GetSize() * entries.size();
	}

	std::ostream& operator<<(std::ostream& os, const TextureAtlasSegment& ta)
	{
		uint32_t numEntries = ta.entries.size();

		os.write((char *)&ta.type,			sizeof(ta.type));
		os.write(ta.name,					sizeof(ta.name));
		os.write(ta.texture,				sizeof(ta.texture));
		os.write((const char*)&numEntries,	sizeof(numEntries));

		for (const AtlasEntry& entry : ta.entries)
		{
			os.write(entry.alias, sizeof(entry.alias));
			os.write((char *) glm::value_ptr(entry.bounds), sizeof(entry.bounds));
		}

		return os;
	}
}