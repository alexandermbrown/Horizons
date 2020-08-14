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
		
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> image = NULL;
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> ttf = NULL;

		{
			std::ifstream ttfFile(basePath.parent_path() / fontPath, std::ios::in | std::ios::binary);
			if (!ttfFile.is_open()) {
				throw "Failed to open ttf file.\n";
			}

			ttfFile.ignore(std::numeric_limits<std::streamsize>::max());
			size_t fileSize = ttfFile.gcount();
			ttfFile.clear();
			ttfFile.seekg(0, std::ios_base::beg);

			uint8_t* fileData;
			ttf = builder.CreateUninitializedVector(fileSize, &fileData);

			ttfFile.read((char*)fileData, fileSize);
			ttfFile.close();
		}

		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>> glyphs_offset = NULL;
		float distance_gradient;

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

					distance_gradient = font->distance_gradient();
					auto imageVector = font->image();
					image = builder.CreateVector(imageVector->data(), imageVector->size());

					const auto* glyphsVector = font->glyphs();
					for (auto cachedGlyph : *glyphsVector)
					{
						glyphs.push_back(Assets::CreateGlyphEntry(builder, cachedGlyph->codepoint(), cachedGlyph->texture_offset()));
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
					throw "The number of glyphs in font is greater than what can be fit in the image size.";

				unsigned long charcode;
				uint32_t codepoint;
				long count = 0;
				std::cout << "Loading font " << name << " with " << numGlyphs << " glyphs.\n";
				std::cout << "[" << count << " / " << numGlyphs << "]\r";

				msdfgen::Bitmap<float, 3> atlas(textureWidth, textureWidth);

				flatbuffers::FlatBufferBuilder cacheBuidler(1024);
				auto cache_name = cacheBuidler.CreateString(name);
				std::vector<flatbuffers::Offset<Assets::GlyphEntry>> glyphs, cache_glyphs;

				msdfgen::FontMetrics metrics;
				msdfgen::getFontMetrics(metrics, font);
				distance_gradient = (float)(1.953125 / metrics.emSize);
				
				charcode = msdfgen::getFirstChar(font, &codepoint);
				while (codepoint != 0 && count < maxGliphs)
				{
					msdfgen::Shape shape;
					if (msdfgen::loadGlyph(shape, font, charcode))
					{
						shape.normalize();
						msdfgen::edgeColoringSimple(shape, 3.0);
						msdfgen::Bitmap<float, 3> msdf(glyphWidth, glyphWidth);

						msdfgen::generateMSDF(msdf, shape, 4.0, msdfgen::Vector2(0.6f * (float)glyphWidth / (float)metrics.emSize), msdfgen::Vector2(4.0, 10.0));

						// Combine images.
						for (int y = 0; y < glyphWidth; y++) {
							for (int x = 0; x < glyphWidth; x++) {
								for (uint32_t channel = 0; channel < 3; channel++) {
									*(atlas((glyphWidth * count) % textureWidth + x, (int)std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth + y) + channel * sizeof(float))
										= *(msdf(x, y) + channel * sizeof(float));
								}
							}
						}

						Assets::Vec2 textureOffset = Assets::Vec2(((glyphWidth * count) % textureWidth) / float(textureWidth),
							(std::floor((float)(glyphWidth * count) / (float)textureWidth) * glyphWidth) / (float)textureWidth);

						glyphs.push_back(Assets::CreateGlyphEntry(builder, codepoint, &textureOffset));

						cache_glyphs.push_back(Assets::CreateGlyphEntry(cacheBuidler, codepoint, &textureOffset));

						if (count % 10 == 0)
							std::cout << "[" << count << " / " << numGlyphs << "]\r";
					}
					else throw std::stringstream("Failed to load character ") << charcode << " in font " << name << ".\n";

					count++;
					charcode = msdfgen::getNextChar(font, charcode, &codepoint);
				}

				std::vector<uint8_t> imageData;
				msdfgen::savePng(atlas, imageData);
				msdfgen::destroyFont(font);
				glyphs_offset = builder.CreateVector(glyphs);
				image = builder.CreateVector(imageData);

				auto cache_glyphs_offset = cacheBuidler.CreateVector(cache_glyphs);
				auto cache_image_offset = cacheBuidler.CreateVector(imageData);
				
				auto cache_font = Assets::CreateFont(cacheBuidler, cache_name, glyphWidth, textureWidth, distance_gradient, cache_glyphs_offset, cache_image_offset);
				cacheBuidler.Finish(cache_font, "FONT");

				std::ofstream cacheFile(cachePath.string(), std::ios::out | std::ios::trunc | std::ios::binary);
				if (!cacheFile.is_open() || !cacheFile.good()) {
					throw "Failed to write cache file!";
				}

				cacheFile.write((const char*)cacheBuidler.GetBufferPointer(), cacheBuidler.GetSize());
				cacheFile.close();

			}
			else throw "Failed to load font.";
		}
		std::cout << "[ done! ]         \n";

		return Assets::CreateFont(builder, name_offset, glyphWidth, textureWidth, distance_gradient, glyphs_offset, image, ttf);
	}
}