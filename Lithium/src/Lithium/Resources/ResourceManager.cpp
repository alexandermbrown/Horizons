#include "lipch.h"
#include "ResourceManager.h"

#include <fstream>
#include "zstr/zstr.hpp"
#include "Serial/Segment.h"

#define LI_READ_FILE(file, ptr, size, pos) file.read(ptr, size); pos += inFile.gcount()

namespace li
{
	Scope<ResourceManager> ResourceManager::s_Instance = CreateScope<ResourceManager>();

	ResourceManager::ResourceManager()
		: m_Textures(), m_Shaders()
	{
	}

	void ResourceManager::InitImpl(const std::string& labFilePath)
	{
		LI_CORE_INFO("Loading asset base {}...", labFilePath);
		zstr::ifstream inFile(labFilePath, std::ios::in | std::ios::binary);

		if (!inFile.good()) {
			LI_CORE_ERROR("Error opening lithium asset base {}", labFilePath);
			return;
		}

		size_t pos = 0;

		// Read file header.
		HeaderSerial header;
		uint32_t signature;
		LI_READ_FILE(inFile, (char*)&signature, sizeof(signature), pos);
		LI_READ_FILE(inFile, (char*)&header.fileSize, sizeof(header.fileSize), pos);
		LI_READ_FILE(inFile, (char*)&header.infoTableOffset, sizeof(header.infoTableOffset), pos);

		if (signature != 0x42414c2bU) {
			LI_CORE_ERROR("Lithium asset base {} has an incorrect header.", labFilePath);
			return;
		}

		// Read in list of assets.
		while (pos < header.infoTableOffset) {

			SegmentType segmentType;
			LI_READ_FILE(inFile, (char*)&segmentType, sizeof(segmentType), pos);

			switch (segmentType)
			{
			case SegmentType::Texture2D:
			{
				char name[64];
				li::FilterType min_filter;
				li::FilterType mag_filter;
				li::WrapType wrap_s;
				li::WrapType wrap_t;
				size_t imageSize;
				uint8_t* imageData;

				LI_READ_FILE(inFile, (char *)&name,			sizeof(name),		pos);
				LI_READ_FILE(inFile, (char *)&min_filter,	sizeof(min_filter), pos);
				LI_READ_FILE(inFile, (char *)&mag_filter,	sizeof(mag_filter), pos);
				LI_READ_FILE(inFile, (char *)&wrap_s,		sizeof(wrap_s),		pos);
				LI_READ_FILE(inFile, (char *)&wrap_t,		sizeof(wrap_t),		pos);
				LI_READ_FILE(inFile, (char *)&imageSize,	sizeof(imageSize),	pos);
				
				imageData = new uint8_t[imageSize];
				LI_READ_FILE(inFile, (char*)imageData, imageSize, pos);

				Ref<Texture2D> texture = Texture2D::Create(imageSize, imageData, wrap_s, wrap_t,
					min_filter, mag_filter);
				m_Textures[name] = texture;

				delete[] imageData;
				break;
			}
			case SegmentType::Shader:
			{
				char name[64];
				size_t shaderSize;

				LI_READ_FILE(inFile, (char*)&name, sizeof(name), pos);
				LI_READ_FILE(inFile, (char*)&shaderSize, sizeof(shaderSize), pos);

				char* shaderData = new char[shaderSize + 1];
				LI_READ_FILE(inFile, (char*)shaderData, shaderSize, pos);
				shaderData[shaderSize] = '\0';

				Ref<Shader> shader = Shader::Create(name, shaderData);
				m_Shaders[name] = shader;

				delete[] shaderData;
				break;
			}
			case SegmentType::TextureAtlas:
			{
				char name[64];
				char texture[64];
				uint32_t numEntries;
				std::unordered_map<std::string, glm::vec4> entries;

				LI_READ_FILE(inFile, (char*)&name, sizeof(name), pos);
				LI_READ_FILE(inFile, (char*)&texture, sizeof(texture), pos);
				LI_READ_FILE(inFile, (char*)&numEntries, sizeof(numEntries), pos);

				for (uint32_t i = 0; i < numEntries; i++)
				{
					glm::vec4 bounds;
					char alias[64];
					LI_READ_FILE(inFile, alias, sizeof(alias), pos);
					LI_READ_FILE(inFile, (char*)&bounds, sizeof(bounds), pos);

					entries[alias] = bounds;
				}

				if (m_Textures.count(texture) < 1)
					LI_CORE_ERROR("Texture {} not found for atlas {}", texture, name);
				else
					m_TextureAtlases[name] = CreateRef<TextureAtlas>(m_Textures[texture], entries);
				break;
			}
			case SegmentType::Font:
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

				LI_READ_FILE(inFile, (char*)&name, sizeof(name), pos);

				LI_READ_FILE(inFile, (char*)&glyphWidth, sizeof(glyphWidth), pos);
				LI_READ_FILE(inFile, (char*)&textureWidth, sizeof(textureWidth), pos);

				LI_READ_FILE(inFile, (char*)&emSize				, sizeof(emSize)			, pos);
				LI_READ_FILE(inFile, (char*)&ascenderY			, sizeof(ascenderY)			, pos);
				LI_READ_FILE(inFile, (char*)&descenderY			, sizeof(descenderY)		, pos);
				LI_READ_FILE(inFile, (char*)&lineHeight			, sizeof(lineHeight)		, pos);
				LI_READ_FILE(inFile, (char*)&underlineY			, sizeof(underlineY)		, pos);
				LI_READ_FILE(inFile, (char*)&underlineThickness, sizeof(underlineThickness)	, pos);

				LI_READ_FILE(inFile, (char*)&numGlyphs, sizeof(numGlyphs), pos);
				for (int i = 0; i < numGlyphs; i++)
				{
					Glyph glyph;
					LI_READ_FILE(inFile, (char*)&glyph.Character, sizeof(glyph.Character), pos);
					LI_READ_FILE(inFile, (char*)&glyph.TextureOffset, sizeof(glyph.TextureOffset), pos);

					LI_READ_FILE(inFile, (char*)&glyph.Width, sizeof(glyph.Width), pos);
					LI_READ_FILE(inFile, (char*)&glyph.Height, sizeof(glyph.Height), pos);

					LI_READ_FILE(inFile, (char*)&glyph.HorizontalAdvance, sizeof(glyph.HorizontalAdvance), pos);
					LI_READ_FILE(inFile, (char*)&glyph.BearingX, sizeof(glyph.BearingX), pos);
					LI_READ_FILE(inFile, (char*)&glyph.BearingY, sizeof(glyph.BearingY), pos);
					glyphs.push_back(glyph);
				}

				LI_READ_FILE(inFile, (char*)&imageSize, sizeof(imageSize), pos);
				imageData = new unsigned char[imageSize];
				LI_READ_FILE(inFile, (char*)imageData, imageSize, pos);

				FontProperties props;
				props.GlyphWidth			= glyphWidth;
				props.TextureWidth			= textureWidth;
				props.EmSize				= emSize;
				props.AscenderY				= ascenderY;
				props.DescenderY			= descenderY;
				props.LineHeight			= lineHeight;
				props.UnderlineY			= underlineY;
				props.UnderlineThickness	= underlineThickness;

				Ref<Texture2D> texture = Texture2D::Create(imageSize, imageData, WrapType::ClampToEdge, WrapType::ClampToEdge,
					FilterType::Linear, FilterType::Linear);

				m_Fonts[name] = CreateRef<Font>(name, props, glyphs, texture);

				delete[] imageData;
				break;
			}
			default:
				LI_CORE_ERROR("Unknown asset type!");
				break;
			}
		}

		LI_CORE_INFO("Successfully loaded lithium asset base {}", labFilePath);
		LI_CORE_INFO("    # Textures        | {}", m_Textures.size());
		LI_CORE_INFO("    # Shaders         | {}", m_Shaders.size());
		LI_CORE_INFO("    # TextureAtlases  | {}", m_TextureAtlases.size());
		LI_CORE_INFO("    # Fonts           | {}", m_Fonts.size());
	}
}