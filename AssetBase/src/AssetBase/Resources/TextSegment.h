#pragma once

#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"

namespace AssetBase
{
	class TextSegment : public Segment
	{
	public:
		char name[64];
		size_t fileSize;
		char* textData;

		TextSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath);
		virtual ~TextSegment();

		virtual size_t GetSize() override;
	};

	std::ostream& operator<<(std::ostream& os, const TextSegment& t);
}