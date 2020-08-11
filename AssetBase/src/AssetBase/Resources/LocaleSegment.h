#pragma once

#include <filesystem>

#include "rapidxml/rapidxml.hpp"
#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace AssetBase
{
	class LocaleSegment
	{
	public:
		static flatbuffers::Offset<Assets::Locale> Serialize(rapidxml::xml_node<>* localeNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode);
	};
}