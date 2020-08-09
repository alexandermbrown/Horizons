#pragma once

#include "Lithium/Renderer/Font.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class FontArgs : public ResourceArgs
	{
	public:
		FontArgs(zstr::ifstream* inFile, size_t* pos);

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