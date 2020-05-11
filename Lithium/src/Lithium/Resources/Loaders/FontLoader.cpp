#include "lipch.h"
#include "FontLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	Ref<Font> LoadFont(std::string* outname, zstr::ifstream* inFile, size_t* pos)
	{
		char name[64];
		uint16_t glyphWidth = 0;
		uint16_t textureWidth = 0;

		float emSize;
		float ascenderY, descenderY;
		float lineHeight;
		float underlineY, underlineThickness;

		uint32_t numGlyphs;
		std::vector<Glyph> glyphs;

		uint32_t imageSize;
		uint8_t* imageData;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);

		LI_READ_FILE(*inFile, (char*)&glyphWidth, sizeof(glyphWidth), *pos);
		LI_READ_FILE(*inFile, (char*)&textureWidth, sizeof(textureWidth), *pos);

		LI_READ_FILE(*inFile, (char*)&emSize, sizeof(emSize), *pos);
		LI_READ_FILE(*inFile, (char*)&ascenderY, sizeof(ascenderY), *pos);
		LI_READ_FILE(*inFile, (char*)&descenderY, sizeof(descenderY), *pos);
		LI_READ_FILE(*inFile, (char*)&lineHeight, sizeof(lineHeight), *pos);
		LI_READ_FILE(*inFile, (char*)&underlineY, sizeof(underlineY), *pos);
		LI_READ_FILE(*inFile, (char*)&underlineThickness, sizeof(underlineThickness), *pos);

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
			glyphs.push_back(glyph);
		}

		LI_READ_FILE(*inFile, (char*)&imageSize, sizeof(imageSize), *pos);
		imageData = new unsigned char[imageSize];
		LI_READ_FILE(*inFile, (char*)imageData, imageSize, *pos);

		FontProperties props;
		props.GlyphWidth = glyphWidth;
		props.TextureWidth = textureWidth;
		props.EmSize = emSize;
		props.AscenderY = ascenderY;
		props.DescenderY = descenderY;
		props.LineHeight = lineHeight;
		props.UnderlineY = underlineY;
		props.UnderlineThickness = underlineThickness;

		Ref<Texture2D> texture = Texture2D::Create(imageSize, imageData, WrapType::ClampToEdge, WrapType::ClampToEdge,
			FilterType::Linear, FilterType::Linear);
		delete[] imageData;

		*outname = std::string(name);

		return CreateRef<Font>(name, props, glyphs, texture);
	}
}