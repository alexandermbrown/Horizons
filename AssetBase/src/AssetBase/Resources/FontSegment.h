#pragma once

#include "Segment.h"

#include <stdint.h>
#include <filesystem>
#include "rapidxml/rapidxml.hpp"

#include "glm/glm.hpp"
#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"

namespace AssetBase
{
	struct GlyphEntry
	{
		wchar_t character;
		glm::vec2 textureOffset;

		float width;
		float height;

		float horizontalAdvance;
		float bearingX;
		float bearingY;

		inline static size_t GetSize() {
			return sizeof(wchar_t) + sizeof(glm::vec2) + sizeof(float) * 5;
		}
	};

	class FontSegment : public Segment
	{
	public:
		static void Init();
		static void Shutdown();

	public:
		char name[64];
		uint16_t glyphWidth = 0;
		uint16_t textureWidth = 0;

		float emSize;
		float ascenderY, descenderY;
		float lineHeight;
		float underlineY, underlineThickness;

		uint32_t numGlyphs;
		GlyphEntry* glyphs;

		uint32_t imageSize;
		std::vector<uint8_t> imageData;

		FontSegment(rapidxml::xml_node<>* fontNode, const std::filesystem::path& basePath, bool debugMode);
		virtual ~FontSegment();
		virtual size_t GetSize() override;
	};

	std::ostream& operator<<(std::ostream& os, const FontSegment& f);
}