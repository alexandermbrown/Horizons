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
		size_t fileSize;
		char* fileData;

		AudioSegment(rapidxml::xml_node<>* audioNode, const std::filesystem::path& basePath);
		virtual ~AudioSegment();

		virtual size_t GetSize() override;
	};

	std::ostream& operator<<(std::ostream& os, const AudioSegment& s);
}