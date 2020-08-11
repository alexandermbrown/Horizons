#pragma once

#include "Segment.h"


#include "rapidxml/rapidxml.hpp"

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"

namespace AssetBase
{
	class ShaderSegment
	{
	public:
		static flatbuffers::Offset<Assets::Shader> Serialize(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode);
	};
}