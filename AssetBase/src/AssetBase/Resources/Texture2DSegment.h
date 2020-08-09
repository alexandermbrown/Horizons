#pragma once

#include "Lithium/Renderer/RendererEnums.h"
#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"

namespace AssetBase
{
	class Texture2DSegment : public Segment
	{
	public:
		char name[64];
		li::FilterType min_filter;
		li::FilterType mag_filter;
		li::WrapType wrap_s;
		li::WrapType wrap_t;
		size_t imageSize;
		uint8_t* imageData;

		Texture2DSegment(rapidxml::xml_node<>* textureNode, const std::filesystem::path& basePath, bool debugMode);
		virtual ~Texture2DSegment();
		virtual size_t GetSize() override;
	};
	std::ostream& operator<<(std::ostream& os, const Texture2DSegment& t);

}