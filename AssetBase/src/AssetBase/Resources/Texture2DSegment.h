#pragma once

#include <filesystem>

#include "rapidxml/rapidxml.hpp"
#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace AssetBase
{
	class Texture2DSegment
	{
	public:
		static flatbuffers::Offset<Assets::Texture2D> Serialize(rapidxml::xml_node<>* textureNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode);
	};
}