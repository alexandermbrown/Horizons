#include "pch.h"
#include "TextureAtlasSegment.h"

#include "Helpers.h"

namespace AssetBase
{
	flatbuffers::Offset<Assets::TextureAtlas> TextureAtlasSegment::Serialize(rapidxml::xml_node<>* atlasNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode)
	{
		flatbuffers::Offset<flatbuffers::String> name = NULL;
		if (auto* name_attr = atlasNode->first_attribute("name"))
		{
			name = builder.CreateString(name_attr->value());
			std::cout << "Loading texture atlas '" << name_attr->value() << "' ... ";
		}
		else throw "Attribute 'name' not found in texture atlas.";

		flatbuffers::Offset<flatbuffers::String> texture = NULL;
		if (auto* texture_node = atlasNode->first_node("texture"))
		{
			texture = builder.CreateString(texture_node->value());
		}
		else throw "Node <texture> not found in texture atlas.";

		std::vector<flatbuffers::Offset<Assets::AtlasEntry>> entries;
		if (auto* entries_node = atlasNode->first_node("entries"))
		{
			for (rapidxml::xml_node<>* entryNode = entries_node->first_node(); entryNode; entryNode = entryNode->next_sibling())
			{
				flatbuffers::Offset<flatbuffers::String> alias = NULL;

				if (auto* alias_attr = entryNode->first_attribute("alias"))
					alias = builder.CreateString(alias_attr->value());
				else throw "Attribute 'alias' not found in texture atlas entry.";

				float x, y, width, height;

				if (auto* attr = entryNode->first_attribute("x"))
					x = Helpers::StringToFloat(attr->value(), "entry.x");
				else throw "Attribute 'x' not found in texture atlas entry.";

				if (auto* attr = entryNode->first_attribute("y"))
					y = Helpers::StringToFloat(attr->value(), "entry.y");
				else throw "Attribute 'y' not found in texture atlas entry.";

				if (auto* attr = entryNode->first_attribute("width"))
					width = Helpers::StringToFloat(attr->value(), "entry.width");
				else throw "Attribute 'width' not found in texture atlas entry.";

				if (auto* attr = entryNode->first_attribute("height"))
					height = Helpers::StringToFloat(attr->value(), "entry.height");
				else throw "Attribute 'height' not found in texture atlas entry.";

				Assets::Vec4 bounds = Assets::Vec4(x, y, width, height);
				entries.push_back(Assets::CreateAtlasEntry(builder, alias, &bounds));
			}
		}
		auto entries_offset = builder.CreateVector(entries);

		std::cout << "done.\n";
		return Assets::CreateTextureAtlas(builder, name, texture, entries_offset);
	}
}