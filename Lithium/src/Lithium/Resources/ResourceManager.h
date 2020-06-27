#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/Shader.h"
#include "Lithium/Renderer/TextureAtlas.h"
#include "Lithium/Renderer/Font.h"
#include "Lithium/Audio/Audio.h"

#include <unordered_map>

namespace li
{
	class ResourceManager
	{
	public:

		static void Init(const std::string& labFilePath);
		static void Shutdown();

		template<typename T>
		static Ref<T> Get(const std::string& name)
		{
			return nullptr;
		}

		template<>
		static Ref<Texture2D> Get<Texture2D>(const std::string& name)
		{
			LI_CORE_ASSERT(s_Data->Textures.find(name) != s_Data->Textures.end(), "Texture not found.");
			return s_Data->Textures.at(name);
		}

		template<>
		static Ref<Shader> Get<Shader>(const std::string& name)
		{
			LI_CORE_ASSERT(s_Data->Shaders.find(name) != s_Data->Shaders.end(), "Shader not found.");
			return s_Data->Shaders.at(name);
		}

		template<>
		static Ref<TextureAtlas> Get<TextureAtlas>(const std::string& name)
		{
			LI_CORE_ASSERT(s_Data->TextureAtlases.find(name) != s_Data->TextureAtlases.end(), "Texture atlas not found.");
			return s_Data->TextureAtlases.at(name);
		}

		template<>
		static Ref<Font> Get<Font>(const std::string& name)
		{
			LI_CORE_ASSERT(s_Data->Fonts.find(name) != s_Data->Fonts.end(), "Font not found.");
			return s_Data->Fonts.at(name);
		}

		template<>
		static Ref<Audio> Get<Audio>(const std::string& name)
		{
			LI_CORE_ASSERT(s_Data->Audio.find(name) != s_Data->Audio.end(), "Audio not found.");
			return s_Data->Audio.at(name);
		}

	private:
		struct ResourceData
		{
			std::unordered_map<std::string, Ref<Texture2D>> Textures;
			std::unordered_map<std::string, Ref<Shader>> Shaders;
			std::unordered_map<std::string, Ref<TextureAtlas>> TextureAtlases;
			std::unordered_map<std::string, Ref<Font>> Fonts;
			std::unordered_map<std::string, Ref<Audio>> Audio;
		};
		
		static Scope<ResourceData> s_Data;
	};
}