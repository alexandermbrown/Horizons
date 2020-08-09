#include "lipch.h"
#include "FontLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	FontArgs::FontArgs(zstr::ifstream* inFile, size_t* pos)
		: ResourceArgs(SegmentType::Font), m_Glyphs()
	{
		char name[64];

		uint32_t numGlyphs;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);

		LI_READ_FILE(*inFile, (char*)&m_Props.GlyphWidth, sizeof(m_Props.GlyphWidth), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Props.TextureWidth, sizeof(m_Props.TextureWidth), *pos);

		LI_READ_FILE(*inFile, (char*)&m_Props.EmSize, sizeof(m_Props.EmSize), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Props.AscenderY, sizeof(m_Props.AscenderY), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Props.DescenderY, sizeof(m_Props.DescenderY), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Props.LineHeight, sizeof(m_Props.LineHeight), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Props.UnderlineY, sizeof(m_Props.UnderlineY), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Props.UnderlineThickness, sizeof(m_Props.UnderlineThickness), *pos);

		LI_READ_FILE(*inFile, (char*)&numGlyphs, sizeof(numGlyphs), *pos);
		for (uint32_t i = 0; i < numGlyphs; i++)
		{
			Glyph glyph;
			LI_READ_FILE(*inFile, (char*)&glyph.Character, sizeof(glyph.Character), *pos);
			LI_READ_FILE(*inFile, (char*)&glyph.TextureOffset, sizeof(glyph.TextureOffset), *pos);

			LI_READ_FILE(*inFile, (char*)&glyph.Width, sizeof(glyph.Width), *pos);
			LI_READ_FILE(*inFile, (char*)&glyph.Height, sizeof(glyph.Height), *pos);

			LI_READ_FILE(*inFile, (char*)&glyph.HorizontalAdvance, sizeof(glyph.HorizontalAdvance), *pos);
			LI_READ_FILE(*inFile, (char*)&glyph.BearingX, sizeof(glyph.BearingX), *pos);
			LI_READ_FILE(*inFile, (char*)&glyph.BearingY, sizeof(glyph.BearingY), *pos);
			m_Glyphs.push_back(glyph);
		}

		LI_READ_FILE(*inFile, (char*)&m_ImageSize, sizeof(m_ImageSize), *pos);
		m_ImageData = new unsigned char[m_ImageSize];
		LI_READ_FILE(*inFile, (char*)m_ImageData, m_ImageSize, *pos);

		m_Name = name;
	}

	FontArgs::~FontArgs()
	{
		delete[] m_ImageData;
	}
}