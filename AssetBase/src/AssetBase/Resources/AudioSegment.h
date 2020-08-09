#pragma once

#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"

namespace AssetBase
{
	class AudioSegment : public Segment
	{
	public:
		char name[64];
		uint32_t fileSize;
		char* fileData;

		AudioSegment(rapidxml::xml_node<>* audioNode, const std::filesystem::path& basePath, bool debugMode);
		virtual ~AudioSegment();

		virtual size_t GetSize() override;
	};

	std::ostream& operator<<(std::ostream& os, const AudioSegment& s);
}