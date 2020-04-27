#include "FontSegment.h"

#include <fstream>
#include <sstream>

#include "Helpers.h"
#include "glm/gtc/type_ptr.hpp"

namespace AssetBase
{
	msdfgen::FreetypeHandle* ft;
	void FontSegment::Init()
	{
		ft = msdfgen::initializeFreetype();
	}

	void FontSegment::Shutdown()
	{
		msdfgen::deinitializeFreetype(ft);
	}

	FontSegment::FontSegment(rapidxml::xml_node<>* fontNode, const std::filesystem::path& basePath)
		: Segment(SegmentType::Font)
	{
		name[0] = '\0';
		for (rapidxml::xml_attribute<>* attr = fontNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			if (!strcmp(attr->name(), "name")) {
				strcpy_s(name, attr->value());
				break;
			}
		}
		if (strlen(name) == 0)
			throw "Attribute 'name' not found in font.";

		std::string fontPath;
		for (rapidxml::xml_node<>* node = fontNode->first_node(); node; node = node->next_sibling())
		{
			const char* name = node->name();
			const char* value = node->value();
			if (!strcmp(name, "source"))
				fontPath = value;

			else if (!strcmp(name, "glyph_width"))
				glyphWidth = Helpers::StringToInt(value, "glyph_width");

			else if (!strcmp(name, "texture_width"))
				textureWidth = Helpers::StringToInt(value, "texture_width");
			
		}
		if (fontPath.empty())
			throw "Missing <source> in font.\n";

		if (glyphWidth == 0)
			throw "Missing <glyph_width> in font.\n";

		if (textureWidth == 0)
			throw "Missing <texture_width> in font.\n";

		if (ft)
		{
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontPath.c_str());
			if (font)
			{
				numGlyphs = msdfgen::getGlyphCount(font);
				long maxGliphs = (textureWidth * textureWidth) / (glyphWidth * glyphWidth);

				if (numGlyphs > maxGliphs)
					throw std::stringstream("The number of glyphs in font ") << numGlyphs << " is greater than the maximum of " << numGlyphs << ".\n";


				glyphs = new GlyphEntry[numGlyphs];

				unsigned long charcode;
				uint32_t gindex;
				uint32_t count = 0;
				std::cout << "Loading font " << name << " with " << numGlyphs << " glyphs.\n";
				std::cout << "[" << count << " / " << numGlyphs << "]\r";

				msdfgen::Bitmap<float, 3> atlas(textureWidth, textureWidth);

				msdfgen::FontMetrics metrics;
				msdfgen::getFontMetrics(metrics, font);
				emSize = metrics.emSize;
				ascenderY = metrics.ascenderY;
				descenderY = metrics.descenderY;
				lineHeight = metrics.lineHeight;
				underlineY = metrics.underlineY;
				underlineThickness = metrics.underlineThickness;

				charcode = msdfgen::getFirstChar(font, &gindex);
				while (gindex != 0 && count < maxGliphs)
				{
					msdfgen::Shape shape;
					if (msdfgen::loadGlyph(shape, font, charcode))
					{
						shape.normalize();
						msdfgen::edgeColoringSimple(shape, 3.0);
						msdfgen::Bitmap<float, 3> msdf(glyphWidth, glyphWidth);

						msdfgen::generateMSDF(msdf, shape, 12.0, msdfgen::Vector2(glyphWidth * 0.75 / emSize), msdfgen::Vector2(4.0, 8.0));

						// Combine images.
						for (int y = 0; y < glyphWidth; y++) {
							for (int x = 0; x < glyphWidth; x++) {
								for (int channel = 0; channel < 3; channel++) {
									*(atlas((glyphWidth * count) % textureWidth + x, std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth + y) + channel * sizeof(float))
										= *(msdf(x, y) + channel * sizeof(float));
								}
							}
						}

						GlyphEntry entry;
						msdfgen::GlyphMetrics metrics;
						msdfgen::getGlyphMetrics(metrics, font);
						glyphs[count].character = charcode;
						glyphs[count].textureOffset = glm::vec2(((glyphWidth * count) % textureWidth) / float(textureWidth),
							(std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth) / (float)textureWidth);

						glyphs[count].width = metrics.width;
						glyphs[count].height = metrics.height;

						glyphs[count].horizontalAdvance = metrics.horiAdvance;
						glyphs[count].bearingX = metrics.horiBearingX;
						glyphs[count].bearingY = metrics.horiBearingY;

						std::cout << "[" << count << " / " << numGlyphs << "]\r";
					}
					else throw std::stringstream("Failed to load character ") << charcode << " in font " << name << ".\n";

					count++;
					charcode = msdfgen::getNextChar(font, charcode, &gindex);
				}
				std::cout << "[ done! ]         \n";

				
				msdfgen::savePng(atlas, "jeff.png");
				msdfgen::savePng(atlas, imageData);
				imageSize = sizeof(uint8_t) * imageData.size();

				msdfgen::destroyFont(font);
			}
			else throw std::stringstream("Failed to load font '") << name << "'\n";
		}
		else throw "Failed to initialize freetype.";
	}

	FontSegment::~FontSegment()
	{
		delete[] glyphs;
	}

	size_t FontSegment::GetSize()
	{
		return sizeof(type) + sizeof(name) + sizeof(uint16_t) * 2 + sizeof(float) * 6 + 
			sizeof(numGlyphs) + numGlyphs * GlyphEntry::GetSize() + sizeof(imageSize) + imageSize;
	}

	std::ostream& operator<<(std::ostream& os, const FontSegment& f)
	{
		os.write((const char*)&f.type, sizeof(f.type));
		os.write(f.name, sizeof(f.name));

		os.write((const char*)&f.glyphWidth, sizeof(f.glyphWidth));
		os.write((const char*)&f.textureWidth, sizeof(f.textureWidth));

		os.write((const char*)&f.emSize, sizeof(f.emSize));
		os.write((const char*)&f.ascenderY, sizeof(f.ascenderY));
		os.write((const char*)&f.descenderY, sizeof(f.descenderY));

		os.write((const char*)&f.lineHeight, sizeof(f.lineHeight));
		os.write((const char*)&f.underlineY, sizeof(f.underlineY));
		os.write((const char*)&f.underlineThickness, sizeof(f.underlineThickness));

		os.write((const char*)&f.numGlyphs, sizeof(f.numGlyphs));
		for (int i = 0; i < f.numGlyphs; i++)
		{
			os.write((const char*)&f.glyphs[i].character					, sizeof(wchar_t));
			os.write((const char*)glm::value_ptr(f.glyphs[i].textureOffset)	, sizeof(glm::vec2));

			os.write((const char*)&f.glyphs[i].width						, sizeof(float));
			os.write((const char*)&f.glyphs[i].height						, sizeof(float));

			os.write((const char*)&f.glyphs[i].horizontalAdvance			, sizeof(float));
			os.write((const char*)&f.glyphs[i].bearingX						, sizeof(float));
			os.write((const char*)&f.glyphs[i].bearingY						, sizeof(float));
		}

		os.write((const char*)&f.imageSize, sizeof(f.imageSize));
		for (int i = 0; i < f.imageData.size(); i++)
		{
			os.write((const char*)&f.imageData.at(i), sizeof(uint8_t));
		}

		return os;
	}
}