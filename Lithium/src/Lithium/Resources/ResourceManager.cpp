#include "lipch.h"
#include "ResourceManager.h"

#include <fstream>
#include "zstr/zstr.hpp"
#include "Serial/Segment.h"
#include "Loaders/TextureLoader.h"
#include "Loaders/ShaderLoader.h"
#include "Loaders/TextureAtlasLoader.h"
#include "Loaders/FontLoader.h"
#include "Loaders/AudioLoader.h"
#include "Loaders/LocaleLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) file.read(ptr, size); pos += file.gcount()

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
				std::string name;
				Ref<Texture2D> texture = LoadTexture2D(&name, &inFile, &pos);
				m_Textures[name] = texture;
				break;
			}
			case SegmentType::Shader:
			{
				std::string name;
				Ref<Shader> shader = LoadShader(&name, &inFile, &pos);
				m_Shaders[name] = shader;
				break;
			}
			case SegmentType::TextureAtlas:
			{
				std::string name;
				Ref<TextureAtlas> atlas = LoadTextureAtlas(m_Textures, &name, &inFile, &pos);
				if (atlas) {
					m_TextureAtlases[name] = atlas;
				}
				break;
			}
			case SegmentType::Font:
			{
				std::string name;
				Ref<Font> font = LoadFont(&name, &inFile, &pos);
				m_Fonts[name] = font;
				break;
			}
			case SegmentType::Audio:
			{
				std::string name;
				Ref<Audio> audio = LoadAudio(&name, &inFile, &pos);
				m_Audio[name] = audio;
				break;
			}
			case SegmentType::Locale:
			{
				std::string name;
				Ref<Locale> locale = LoadLocale(&name, &inFile, &pos);
				Localization::AddLocale(locale);
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
		LI_CORE_INFO("    # Audio           | {}", m_Audio.size());
		LI_CORE_INFO("    # Locales         | {}", Localization::GetLocaleCount());
	}

	void ResourceManager::ShutdownImpl()
	{
		m_Textures.clear();
		m_Shaders.clear();
		m_TextureAtlases.clear();
		m_Fonts.clear();
		m_Audio.clear();
	}
}