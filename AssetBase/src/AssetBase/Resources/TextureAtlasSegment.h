#pragma once

#include "rapidxml/rapidxml.hpp"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

#include <filesystem>

namespace AssetBase
{
	class TextureAtlasSegment
	{
	public:
		static flatbuffers::Offset<Assets::TextureAtlas> Serialize(rapidxml::xml_node<>* atlasNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode);
	};
}