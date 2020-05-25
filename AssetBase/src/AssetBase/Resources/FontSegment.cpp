#include "pch.h"
#include "FontSegment.h"

#include <fstream>
#include <sstream>

#include "Helpers.h"
#include "glm/gtc/type_ptr.hpp"
#include "zstr/zstr.hpp"

namespace AssetBase
{
	msdfgen::FreetypeHandle* ft;
	void FontSegment::Init()
	{
		ft = msdfgen::initializeFreetype();
		if (!ft) throw "Failed to initialize freetype.";
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

		bool useCache = false;
		std::filesystem::path cachePath = "./.lab-cache/fonts/" + std::string(name) + ".cache";
		if (std::filesystem::exists(cachePath))
		{
			zstr::ifstream cacheFile(cachePath.string(), std::ios::in | std::ios::binary);

			uint32_t cache_type;
			char cache_name[64];
			uint16_t cache_glyphWidth = 0;
			uint16_t cache_textureWidth = 0;

			cacheFile.read((char*)&cache_type				, sizeof(cache_type));
			cacheFile.read((char*)&cache_name				, sizeof(cache_name));
			cacheFile.read((char*)&cache_glyphWidth			, sizeof(cache_glyphWidth));
			cacheFile.read((char*)&cache_textureWidth		, sizeof(cache_textureWidth));

			if ((uint32_t)type == cache_type &&
				!strcmp(cache_name, name) &&
				glyphWidth == cache_glyphWidth &&
				textureWidth == cache_textureWidth
				)
			{
				useCache = true;

				cacheFile.read((char*)&emSize, sizeof(emSize));
				cacheFile.read((char*)&ascenderY, sizeof(ascenderY));
				cacheFile.read((char*)&descenderY, sizeof(descenderY));
				cacheFile.read((char*)&lineHeight, sizeof(lineHeight));
				cacheFile.read((char*)&underlineY, sizeof(underlineY));
				cacheFile.read((char*)&underlineThickness, sizeof(underlineThickness));

				cacheFile.read((char*)&numGlyphs, sizeof(numGlyphs));

				std::cout << "Loading font " << name << " with " << numGlyphs << " glyphs ... ";
				glyphs = new GlyphEntry[numGlyphs];

				for (uint32_t i = 0; i < numGlyphs; i++)
				{
					cacheFile.read((char*)&glyphs[i].character, sizeof(glyphs[i].character));
					cacheFile.read((char*)&glyphs[i].textureOffset, sizeof(glyphs[i].textureOffset));

					cacheFile.read((char*)&glyphs[i].width, sizeof(glyphs[i].width));
					cacheFile.read((char*)&glyphs[i].height, sizeof(glyphs[i].height));

					cacheFile.read((char*)&glyphs[i].horizontalAdvance, sizeof(glyphs[i].horizontalAdvance));
					cacheFile.read((char*)&glyphs[i].bearingX, sizeof(glyphs[i].bearingX));
					cacheFile.read((char*)&glyphs[i].bearingY, sizeof(glyphs[i].bearingY));
				}

				cacheFile.read((char*)&imageSize, sizeof(imageSize));
				unsigned char* rawImageData = new unsigned char[imageSize];
				cacheFile.read((char*)rawImageData, imageSize);

				for (uint32_t i = 0; i < imageSize; i++)
				{
					imageData.push_back(rawImageData[i]);
				}
				delete[] rawImageData;

				std::cout << "done.\n";
			}
		}

		if (!useCache)
		{
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontPath.c_str());
			if (font)
			{
				numGlyphs = msdfgen::getGlyphCount(font);
				uint32_t maxGliphs = (textureWidth * textureWidth) / (glyphWidth * glyphWidth);

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
				emSize = (float)metrics.emSize;
				ascenderY = (float)metrics.ascenderY;
				descenderY = (float)metrics.descenderY;
				lineHeight = (float)metrics.lineHeight;
				underlineY = (float)metrics.underlineY;
				underlineThickness = (float)metrics.underlineThickness;

				charcode = msdfgen::getFirstChar(font, &gindex);
				while (gindex != 0 && count < maxGliphs)
				{
					msdfgen::Shape shape;
					if (msdfgen::loadGlyph(shape, font, charcode))
					{
						shape.normalize();
						msdfgen::edgeColoringSimple(shape, 3.0);
						msdfgen::Bitmap<float, 3> msdf(glyphWidth, glyphWidth);

						msdfgen::generateMSDF(msdf, shape, 12.0, msdfgen::Vector2(glyphWidth * 0.75 / emSize), msdfgen::Vector2(4.0, 10.0));

						// Combine images.
						for (uint32_t y = 0; y < glyphWidth; y++) {
							for (uint32_t x = 0; x < glyphWidth; x++) {
								for (uint32_t channel = 0; channel < 3; channel++) {
									*(atlas((glyphWidth * count) % textureWidth + x, (int)std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth + y) + channel * sizeof(float))
										= *(msdf(x, y) + channel * sizeof(float));
								}
							}
						}

						msdfgen::GlyphMetrics metrics;
						msdfgen::getGlyphMetrics(metrics, font);
						glyphs[count].character = (wchar_t)charcode;
						glyphs[count].textureOffset = glm::vec2(((glyphWidth * count) % textureWidth) / float(textureWidth),
							(std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth) / (float)textureWidth);

						glyphs[count].width = (float)metrics.width;
						glyphs[count].height = (float)metrics.height;

						glyphs[count].horizontalAdvance = (float)metrics.horiAdvance;
						glyphs[count].bearingX = (float)metrics.horiBearingX;
						glyphs[count].bearingY = (float)metrics.horiBearingY;

						std::cout << "[" << count << " / " << numGlyphs << "]\r";
					}
					else throw std::stringstream("Failed to load character ") << charcode << " in font " << name << ".\n";

					count++;
					charcode = msdfgen::getNextChar(font, charcode, &gindex);
				}
				std::cout << "[ done! ]         \n";
				
				msdfgen::savePng(atlas, "jeff.png");
				msdfgen::savePng(atlas, imageData);
				imageSize = (uint32_t)(sizeof(uint8_t) * imageData.size());

				msdfgen::destroyFont(font);

				// Write cache file.
				zstr::ofstream cacheFile(cachePath.string(), std::ios::out | std::ios::trunc | std::ios::binary);
				cacheFile << *this;
			}
			else throw std::stringstream("Failed to load font '") << name << "'\n";
		}
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
		for (uint32_t i = 0; i < f.numGlyphs; i++)
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