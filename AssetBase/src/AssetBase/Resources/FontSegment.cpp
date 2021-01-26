#include "pch.h"
#include "FontSegment.h"

#include "Convert.h"
#include "OptionalField.h"

#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"

namespace AssetBase
{
	msdfgen::FreetypeHandle* ft = nullptr;
	void InitFreetype()
	{
		ft = msdfgen::initializeFreetype();
		if (!ft) throw "Failed to initialize freetype.";
	}

	void ShutdownFreetype()
	{
		msdfgen::deinitializeFreetype(ft);
		ft = nullptr;
	}

	flatbuffers::Offset<Assets::Font> SerializeFont(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode)
	{
		flatbuffers::Offset<flatbuffers::String> name_offset = builder.CreateString(name);

		std::filesystem::path font_path = base_path.parent_path() / GetOptionalString(atlas, "path");

		int16_t glyph_width = Convert::StringToInt(GetOptionalString(atlas, "glyph_width"), "glyph_width");
		int16_t texture_width = Convert::StringToInt(GetOptionalString(atlas, "texture_width"), "texture_width");
		
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> image = NULL;
		flatbuffers::Offset<flatbuffers::Vector<uint8_t>> ttf = NULL;

		// Load ttf file.
		{
			std::ifstream ttf_file(font_path, std::ios::in | std::ios::binary);
			if (!ttf_file.is_open())
				throw "Failed to open ttf file.\n";

			ttf_file.ignore(std::numeric_limits<std::streamsize>::max());
			size_t file_size = ttf_file.gcount();
			ttf_file.clear();
			ttf_file.seekg(0, std::ios_base::beg);

			uint8_t* file_data;
			ttf = builder.CreateUninitializedVector(file_size, &file_data);

			ttf_file.read((char*)file_data, file_size);
			ttf_file.close();
		}

		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>> glyphs_offset = NULL;
		float distance_gradient;

		bool use_cache = false;
		std::filesystem::path cache_path = "./.lab-cache/fonts/" + name + ".cache";
		if (std::filesystem::exists(cache_path))
		{
			std::ifstream cache_file(cache_path, std::ios::in | std::ios::binary);

			cache_file.seekg(0, std::ios::end);
			int file_size = (int)cache_file.tellg();
			cache_file.seekg(0, std::ios::beg);
			uint8_t* buffer = new uint8_t[file_size];
			cache_file.read((char*)buffer, file_size);
			cache_file.close();

			const Assets::Font* font = flatbuffers::GetRoot<Assets::Font>(buffer);

			if (font->Verify(flatbuffers::Verifier(buffer, file_size)))
			{
				if (name != font->name()->c_str()
					|| glyph_width != font->glyph_width()
					|| texture_width != font->texture_width())
				{
					std::cout << "\nInvalid font cache file! Rebuilding font... ";
				}
				else
				{
					std::vector<flatbuffers::Offset<Assets::GlyphEntry>> glyphs;
					use_cache = true;

					distance_gradient = font->distance_gradient();
					auto image_vector = font->image();
					image = builder.CreateVector(image_vector->data(), image_vector->size());

					const auto* glyphs_vector = font->glyphs();
					for (const Assets::GlyphEntry* cached_glyph : *glyphs_vector)
					{
						glyphs.push_back(Assets::CreateGlyphEntry(builder, cached_glyph->codepoint(), cached_glyph->texture_offset()));
					}
					glyphs_offset = builder.CreateVector(glyphs);
				}
			}
			else std::cout << "\nInvalid font cache file! Rebuilding font... ";

			delete[] buffer;
		}

		if (!use_cache)
		{
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, font_path.u8string().c_str());
			if (!font)
				throw "Failed to load font.";

			long num_glyphs = msdfgen::getGlyphCount(font);
			long max_glyphs = (texture_width * texture_width) / (glyph_width * glyph_width);

			if (num_glyphs > max_glyphs)
				throw "The number of glyphs in font is greater than what can be fit in the image size.";

			unsigned long charcode;
			uint32_t codepoint;
			long count = 0;
			std::cout << "Generating " << num_glyphs << " glyphs.\n";
			std::cout << "[" << count << " / " << num_glyphs << "]\r";

			msdfgen::Bitmap<float, 3> atlas(texture_width, texture_width);

			flatbuffers::FlatBufferBuilder cacheBuidler(1024);
			auto cache_name = cacheBuidler.CreateString(name);
			std::vector<flatbuffers::Offset<Assets::GlyphEntry>> glyphs, cache_glyphs;

			msdfgen::FontMetrics metrics;
			msdfgen::getFontMetrics(metrics, font);
			distance_gradient = (float)(1.953125 / metrics.emSize);
				
			charcode = msdfgen::getFirstChar(font, &codepoint);
			while (codepoint != 0 && count < max_glyphs)
			{
				msdfgen::Shape shape;
				if (msdfgen::loadGlyph(shape, font, charcode))
				{
					shape.normalize();
					msdfgen::edgeColoringSimple(shape, 3.0);
					msdfgen::Bitmap<float, 3> msdf(glyph_width, glyph_width);

					msdfgen::generateMSDF(msdf, shape, 4.0, msdfgen::Vector2(0.6f * (float)glyph_width / (float)metrics.emSize), msdfgen::Vector2(4.0, 10.0));

					// Combine images.
					for (int y = 0; y < glyph_width; y++) {
						for (int x = 0; x < glyph_width; x++) {
							for (uint32_t channel = 0; channel < 3; channel++) {
								*(atlas((glyph_width* count) % texture_width + x, (int)std::floor((float)(glyph_width* count) / (float)texture_width) * glyph_width + y) + channel * sizeof(float))
									= *(msdf(x, y) + channel * sizeof(float));
							}
						}
					}

					Assets::Vec2 textureOffset = Assets::Vec2(((glyph_width * count) % texture_width) / float(texture_width),
						(std::floor((float)(glyph_width * count) / (float)texture_width) * glyph_width) / (float)texture_width);

					glyphs.push_back(Assets::CreateGlyphEntry(builder, codepoint, &textureOffset));

					cache_glyphs.push_back(Assets::CreateGlyphEntry(cacheBuidler, codepoint, &textureOffset));

					if (count % 10 == 0)
						std::cout << "[" << count << " / " << num_glyphs << "]\r";
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
				
			auto cache_font = Assets::CreateFont(cacheBuidler, cache_name, glyph_width, texture_width, distance_gradient, cache_glyphs_offset, cache_image_offset);
			cacheBuidler.Finish(cache_font, "FONT");

			std::filesystem::create_directories(cache_path);
			std::ofstream cache_file(cache_path, std::ios::out | std::ios::trunc | std::ios::binary);
			if (!cache_file.is_open() || !cache_file.good()) {
				throw "Failed to write cache file!";
			}

			cache_file.write((const char*)cacheBuidler.GetBufferPointer(), cacheBuidler.GetSize());
			cache_file.close();
		}

		return Assets::CreateFont(builder, name_offset, glyph_width, texture_width, distance_gradient, glyphs_offset, image, ttf);
	}
}
