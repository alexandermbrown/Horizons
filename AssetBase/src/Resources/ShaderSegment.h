#pragma once

#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"

namespace AssetBase
{
	class ShaderSegment : public Segment
	{
	public:
		char name[64];
		size_t shaderSize;
		char* shaderData;

		ShaderSegment(rapidxml::xml_node<>* shaderNode, const std::filesystem::path& basePath);
		virtual ~ShaderSegment();

		virtual size_t GetSize() override;
	};

	std::ostream& operator<<(std::ostream& os, const ShaderSegment& s);
}