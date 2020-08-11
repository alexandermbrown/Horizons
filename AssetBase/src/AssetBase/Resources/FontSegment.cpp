#include "pch.h"
#include "FontSegment.h"

#include "Helpers.h"

#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"

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

	flatbuffers::Offset<Assets::Font> FontSegment::Serialize(rapidxml::xml_node<>* fontNode, const std::filesystem::path& basePath, flatbuffers::FlatBufferBuilder& builder, bool debugMode)
	{
		flatbuffers::Offset<flatbuffers::String> name_offset = NULL;
		char* name = nullptr;
		if (auto* name_attr = fontNode->first_attribute("name"))
		{
			name = name_attr->value();
			name_offset = builder.CreateString(name);
			std::cout << "Loading font '" << name << "' ... ";
		}
		else throw "Attribute 'name' not found in font.";

		char* fontPath = nullptr;
		if (auto* node = fontNode->first_node("source"))
			fontPath = node->value();
		else throw "Node <source> not found in font.";

		int16_t glyphWidth;
		if (auto* node = fontNode->first_node("glyph_width"))
			glyphWidth = Helpers::StringToInt(node->value(), "glyph_width");
		else throw "Node <glyph_width> not found in font.";

		int16_t textureWidth;
		if (auto* node = fontNode->first_node("texture_width"))
			textureWidth = Helpers::StringToInt(node->value(), "texture_width");
		else throw "Node <texture_width> not found in font.";

		float emSize, ascenderY, descenderY;
		float lineHeight, underlineY, underlineThickness;
		
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> image = NULL;
		
		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>> glyphs_offset = NULL;

		bool useCache = false;
		std::filesystem::path cachePath = "./.lab-cache/fonts/" + std::string(name) + ".cache";
		if (std::filesystem::exists(cachePath))
		{
			std::ifstream cacheFile(cachePath.string(), std::ios::in | std::ios::binary);

			cacheFile.seekg(0, std::ios::end);
			int filesize = (int)cacheFile.tellg();
			cacheFile.seekg(0, std::ios::beg);
			uint8_t* buffer = new uint8_t[filesize];
			cacheFile.read((char*)buffer, filesize);
			cacheFile.close();

			const Assets::Font* font = flatbuffers::GetRoot<Assets::Font>(buffer);

			if (font->Verify(flatbuffers::Verifier(buffer, filesize)))
			{
				if (strcmp(name, font->name()->c_str()) != 0
					|| glyphWidth != font->glyph_width()
					|| textureWidth != font->texture_width())
				{
					std::cout << "\nInvalid font cache file! Rebuilding font... ";
				}
				else
				{
					std::vector<flatbuffers::Offset<Assets::GlyphEntry>> glyphs;
					useCache = true;
					emSize = font->em_size();
					ascenderY = font->ascender_y();
					descenderY = font->descender_y();
					lineHeight = font->line_height();
					underlineY = font->underline_y();
					underlineThickness = font->underline_thickness();

					auto imageVector = font->image();
					image = builder.CreateVector(imageVector->data(), imageVector->size());

					const auto* glyphsVector = font->glyphs();
					for (auto cachedGlyph : *glyphsVector)
					{
						glyphs.push_back(Assets::CreateGlyphEntry(builder, cachedGlyph->character(), cachedGlyph->texture_offset(), cachedGlyph->width(), 
							cachedGlyph->height(), cachedGlyph->horizontal_advance(), cachedGlyph->bearing_x(), cachedGlyph->bearing_y()));
					}
					glyphs_offset = builder.CreateVector(glyphs);
				}
			}
			else std::cout << "\nInvalid font cache file! Rebuilding font... ";

			delete[] buffer;
		}

		if (!useCache)
		{
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontPath);
			if (font)
			{
				long numGlyphs = msdfgen::getGlyphCount(font);
				long maxGliphs = (textureWidth * textureWidth) / (glyphWidth * glyphWidth);

				if (numGlyphs > maxGliphs)
					throw std::stringstream("The number of glyphs in font ") << numGlyphs << " is greater than the maximum of " << numGlyphs << ".\n";

				unsigned long charcode;
				uint32_t gindex;
				long count = 0;
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

				flatbuffers::FlatBufferBuilder cacheBuidler(1024);
				auto cache_name = cacheBuidler.CreateString(name);
				std::vector<flatbuffers::Offset<Assets::GlyphEntry>> glyphs, cache_glyphs;

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
						for (int y = 0; y < glyphWidth; y++) {
							for (int x = 0; x < glyphWidth; x++) {
								for (uint32_t channel = 0; channel < 3; channel++) {
									*(atlas((glyphWidth * count) % textureWidth + x, (int)std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth + y) + channel * sizeof(float))
										= *(msdf(x, y) + channel * sizeof(float));
								}
							}
						}

						msdfgen::GlyphMetrics metrics;
						msdfgen::getGlyphMetrics(metrics, font);
						Assets::Vec2 textureOffset = Assets::Vec2(((glyphWidth * count) % textureWidth) / float(textureWidth),
							(std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth) / (float)textureWidth);

						glyphs.push_back(Assets::CreateGlyphEntry(builder, (int16_t)charcode, &textureOffset, (float)metrics.width, 
							(float)metrics.height, (float)metrics.horiAdvance, (float)metrics.horiBearingX, (float)metrics.horiBearingY));

						cache_glyphs.push_back(Assets::CreateGlyphEntry(cacheBuidler, (int16_t)charcode, &textureOffset, (float)metrics.width,
							(float)metrics.height, (float)metrics.horiAdvance, (float)metrics.horiBearingX, (float)metrics.horiBearingY));

						if (count % 10 == 0)
							std::cout << "[" << count << " / " << numGlyphs << "]\r";
					}
					else throw std::stringstream("Failed to load character ") << charcode << " in font " << name << ".\n";

					count++;
					charcode = msdfgen::getNextChar(font, charcode, &gindex);
				}

				std::vector<uint8_t> imageData;
				msdfgen::savePng(atlas, imageData);
				msdfgen::destroyFont(font);
				glyphs_offset = builder.CreateVector(glyphs);
				image = builder.CreateVector(imageData);

				auto cache_glyphs_offset = cacheBuidler.CreateVector(cache_glyphs);
				auto cache_image_offset = cacheBuidler.CreateVector(imageData);
				
				auto cache_font = Assets::CreateFont(cacheBuidler, cache_name, glyphWidth, textureWidth, emSize,
					ascenderY, descenderY, lineHeight, underlineY, underlineThickness, cache_glyphs_offset, cache_image_offset);
				cacheBuidler.Finish(cache_font, "FONT");

				std::ofstream cacheFile(cachePath.string(), std::ios::out | std::ios::trunc | std::ios::binary);
				if (!cacheFile.is_open() || !cacheFile.good()) {
					throw "Failed to write cache file!";
				}

				cacheFile.write((const char*)cacheBuidler.GetBufferPointer(), cacheBuidler.GetSize());
				cacheFile.close();

			}
			else throw std::stringstream("Failed to load font '") << name << "'\n";
		}
		std::cout << "[ done! ]         \n";

		return Assets::CreateFont(builder, name_offset, glyphWidth, textureWidth, emSize,
			ascenderY, descenderY, lineHeight, underlineY, underlineThickness, glyphs_offset, image);
	}
}