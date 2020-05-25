#pragma once

#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"

namespace AssetBase
{
	class LocaleSegment : public Segment
	{
	public:
		char name[64];

		size_t keysLength;
		char* keys = nullptr;

		size_t valuesLength;
		wchar_t* values = nullptr;

		LocaleSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath);
		virtual ~LocaleSegment();

		virtual size_t GetSize() override;
	};

	std::ostream& operator<<(std::ostream& os, const LocaleSegment& s);
}