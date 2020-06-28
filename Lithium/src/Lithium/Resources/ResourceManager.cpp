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

#include <thread>

#define LI_READ_FILE(file, ptr, size, pos) file.read(ptr, size); pos += file.gcount()

namespace li
{
	Scope<ResourceManager::ResourceData> ResourceManager::s_Data;
	std::atomic<bool> ResourceManager::s_Loaded(false);

	void ResourceManager::LoadAsync(const std::string& labFilePath)
	{
		s_Data = CreateScope<ResourceData>();

		s_Data->LoadThread = std::thread(LoadFile, labFilePath);
	}

	void ResourceManager::Shutdown()
	{
		s_Data->LoadThread.join();
		s_Data.reset();
	}

	bool ResourceManager::DequeueAsset()
	{
		ResourceArgs* resource;
		if (s_Data->ArgsQueue.try_dequeue(resource))
		{
			switch (resource->GetType())
			{
			case SegmentType::Texture2D:
			{
				Texture2DArgs* texture = (Texture2DArgs*)resource;
				s_Data->Textures[texture->GetName()] = texture->Create();
				break;
			}
			case SegmentType::Shader:
			{
				ShaderArgs* shader = (ShaderArgs*)resource;
				s_Data->Shaders[shader->GetName()] = shader->Create();
				break;
			}
			case SegmentType::TextureAtlas:
			{
				TextureAtlasArgs* atlas = (TextureAtlasArgs*)resource;
				s_Data->TextureAtlases[atlas->GetName()] = atlas->Create();
				break;
			}
			case SegmentType::Font:
			{
				FontArgs* font = (FontArgs*)resource;
				s_Data->Fonts[font->GetName()] = font->Create();
				break;
			}
			case SegmentType::Audio:
			{
				AudioArgs* audio = (AudioArgs*)resource;
				s_Data->Audio[audio->GetName()] = audio->Create();
				break;
			}
			case SegmentType::Locale:
			{
				LocaleArgs* locale = (LocaleArgs*)resource;
				Localization::AddLocale(locale->Create());
				break;
			}
			default:
				LI_CORE_ERROR("Unknown asset type!");
				break;
			}

			delete resource;

			return true;
		}
		return false;
	}

	void ResourceManager::PrintInfo()
	{
		LI_CORE_INFO("--------------------------------");
		LI_CORE_INFO("    Lithium Asset Base Info");
		LI_CORE_INFO("--------------------------------");
		LI_CORE_INFO("    # Textures        | {}", s_Data->Textures.size());
		LI_CORE_INFO("    # Shaders         | {}", s_Data->Shaders.size());
		LI_CORE_INFO("    # TextureAtlases  | {}", s_Data->TextureAtlases.size());
		LI_CORE_INFO("    # Fonts           | {}", s_Data->Fonts.size());
		LI_CORE_INFO("    # Audio           | {}", s_Data->Audio.size());
		LI_CORE_INFO("    # Locales         | {}", Localization::GetLocaleCount());
		LI_CORE_INFO("--------------------------------");
	}

	void ResourceManager::LoadFile(std::string labFilePath)
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
		while (pos < header.infoTableOffset)
		{
			SegmentType segmentType;
			LI_READ_FILE(inFile, (char*)&segmentType, sizeof(segmentType), pos);

			switch (segmentType)
			{
			case SegmentType::Texture2D:
				s_Data->ArgsQueue.enqueue(Texture2DArgs::Deserialize(&inFile, &pos));
				break;

			case SegmentType::Shader:
				s_Data->ArgsQueue.enqueue(ShaderArgs::Deserialize(&inFile, &pos));
				break;

			case SegmentType::TextureAtlas:
				s_Data->ArgsQueue.enqueue(TextureAtlasArgs::Deserialize(&inFile, &pos));
				break;
			
			case SegmentType::Font:
				s_Data->ArgsQueue.enqueue(FontArgs::Deserialize(&inFile, &pos));
				break;
			
			case SegmentType::Audio:
				s_Data->ArgsQueue.enqueue(AudioArgs::Deserialize(&inFile, &pos));
				break;
			
			case SegmentType::Locale:
				s_Data->ArgsQueue.enqueue(LocaleArgs::Deserialize(&inFile, &pos));
				break;
			
			default:
				LI_CORE_ERROR("Unknown asset type!");
				break;
			}
		}

		s_Loaded = true;
	}
}