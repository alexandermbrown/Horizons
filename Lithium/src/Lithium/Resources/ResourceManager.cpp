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
	Scope<ResourceManager::ResourceData> ResourceManager::s_Data;

	void ResourceManager::Init(const std::string& labFilePath)
	{
		s_Data = CreateScope<ResourceData>();
		
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
				s_Data->Textures[name] = texture;
				break;
			}
			case SegmentType::Shader:
			{
				std::string name;
				Ref<Shader> shader = LoadShader(&name, &inFile, &pos);
				s_Data->Shaders[name] = shader;
				break;
			}
			case SegmentType::TextureAtlas:
			{
				std::string name;
				Ref<TextureAtlas> atlas = LoadTextureAtlas(s_Data->Textures, &name, &inFile, &pos);
				if (atlas) {
					s_Data->TextureAtlases[name] = atlas;
				}
				break;
			}
			case SegmentType::Font:
			{
				std::string name;
				Ref<Font> font = LoadFont(&name, &inFile, &pos);
				s_Data->Fonts[name] = font;
				break;
			}
			case SegmentType::Audio:
			{
				std::string name;
				Ref<Audio> audio = LoadAudio(&name, &inFile, &pos);
				s_Data->Audio[name] = audio;
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
		LI_CORE_INFO("    # Textures        | {}", s_Data->Textures.size());
		LI_CORE_INFO("    # Shaders         | {}", s_Data->Shaders.size());
		LI_CORE_INFO("    # TextureAtlases  | {}", s_Data->TextureAtlases.size());
		LI_CORE_INFO("    # Fonts           | {}", s_Data->Fonts.size());
		LI_CORE_INFO("    # Audio           | {}", s_Data->Audio.size());
		LI_CORE_INFO("    # Locales         | {}", Localization::GetLocaleCount());
	}

	void ResourceManager::Shutdown()
	{
		s_Data.reset();
	}
}