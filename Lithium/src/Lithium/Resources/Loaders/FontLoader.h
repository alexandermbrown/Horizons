#pragma once

#include "Lithium/Renderer/Font.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class FontArgs : public ResourceArgs
	{
	public:
		static FontArgs* Deserialize(zstr::ifstream* inFile, size_t* pos);

		FontArgs(const std::string& name, uint32_t imageSize, uint8_t* imageData, FontProperties props, std::vector<Glyph>&& glyphs)
			: ResourceArgs(SegmentType::Font, name), m_ImageSize(imageSize), m_ImageData(imageData), m_Props(props), m_Glyphs(glyphs) {}

		virtual ~FontArgs();

		inline Ref<Font> Create()
		{
			Ref<Texture2D> texture = Texture2D::Create(m_ImageSize, m_ImageData, WrapType::ClampToEdge, WrapType::ClampToEdge,
				FilterType::Linear, FilterType::Linear);

			return CreateRef<Font>(m_Name, m_Props, m_Glyphs, texture);
		};

	private:
		uint32_t m_ImageSize;
		uint8_t* m_ImageData;

		FontProperties m_Props;
		std::vector<Glyph> m_Glyphs;
	};
}