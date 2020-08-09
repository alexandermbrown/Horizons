#pragma once

#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"
#include "glm/glm.hpp"

namespace AssetBase
{
	struct AtlasEntry
	{
		char alias[64];
		glm::vec4 bounds;

		static size_t GetSize() {
			return sizeof(alias) + sizeof(bounds);
		}
	};

	class TextureAtlasSegment : public Segment
	{
	public:

		TextureAtlasSegment(rapidxml::xml_node<>* atlasNode, const std::filesystem::path& basePath, bool debugMode);
		virtual ~TextureAtlasSegment() = default;

		virtual size_t GetSize() override;

	public:
		char name[64];
		char texture[64];
		std::vector<AtlasEntry> entries;
	};

	std::ostream& operator<<(std::ostream& os, const TextureAtlasSegment& ta);
}