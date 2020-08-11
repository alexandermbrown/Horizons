#pragma once

#include "rapidxml/rapidxml.hpp"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

#include <filesystem>

namespace AssetBase
{
	class FontSegment
	{
	public:
		static void Init();
		static void Shutdown();

		static flatbuffers::Offset<Assets::Font> Serialize(rapidxml::xml_node<>* fontNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode);
	};
}