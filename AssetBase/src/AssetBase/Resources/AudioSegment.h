#pragma once

#include <filesystem>
#include "rapidxml/rapidxml.hpp"
#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace AssetBase
{
	class AudioSegment
	{
	public:
		static flatbuffers::Offset<Assets::Audio> Serialize(rapidxml::xml_node<>* audioNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode);
	};
}