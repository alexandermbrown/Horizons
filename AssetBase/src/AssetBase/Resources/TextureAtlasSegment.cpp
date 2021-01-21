#include "pch.h"
#include "TextureAtlasSegment.h"

#include "Convert.h"
#include "OptionalField.h"

namespace AssetBase
{
	flatbuffers::Offset<Assets::TextureAtlas> SerializeTextureAtlas(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode)
	{
		std::string texture = GetOptionalString(atlas, "texture");

		YAML::Node entries_node = atlas["entries"];
		if (!entries_node)
			throw std::runtime_error("Atlas " + name + " missing field 'entries'");

		std::vector<flatbuffers::Offset<Assets::AtlasEntry>> entries;
		for (const std::pair<YAML::Node, YAML::Node>& entry : entries_node)
		{
			flatbuffers::Offset<flatbuffers::String> alias = builder.CreateString(entry.first.Scalar());

			float x = Convert::StringToFloat(GetOptionalString(entry.second, "x"), "x");
			float y = Convert::StringToFloat(GetOptionalString(entry.second, "y"), "y");
			float width = Convert::StringToFloat(GetOptionalString(entry.second, "width"), "width");
			float height = Convert::StringToFloat(GetOptionalString(entry.second, "height"), "height");

			Assets::Vec4 bounds(x, y, width, height);
			entries.push_back(Assets::CreateAtlasEntry(builder, alias, &bounds));
		}

		return Assets::CreateTextureAtlasDirect(builder, name.c_str(), texture.c_str(), &entries);
	}
}
