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
		inline static void Init(const std::string& labFilePath) {
			s_Instance->InitImpl(labFilePath);
		}

		inline static void Shutdown() { s_Instance->ShutdownImpl(); }

		template<typename T>
		inline static Ref<T> Get(const std::string& name)
		{
			return s_Instance->GetImpl<T>(name);
		}

		ResourceManager();

	private:
		void InitImpl(const std::string& labFilePath);
		void ShutdownImpl();

		template<typename T>
		Ref<T> GetImpl(const std::string& name) const
		{
			return nullptr;
		}

		template<>
		Ref<Texture2D> GetImpl<Texture2D>(const std::string& name) const
		{
			LI_CORE_ASSERT(m_Textures.find(name) != m_Textures.end(), "Texture not found.");
			return m_Textures.at(name);
		}

		template<>
		Ref<Shader> GetImpl<Shader>(const std::string& name) const
		{
			LI_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found.");
			return m_Shaders.at(name);
		}

		template<>
		Ref<TextureAtlas> GetImpl<TextureAtlas>(const std::string& name) const
		{
			LI_CORE_ASSERT(m_TextureAtlases.find(name) != m_TextureAtlases.end(), "Texture atlas not found.");
			return m_TextureAtlases.at(name);
		}

		template<>
		Ref<Font> GetImpl<Font>(const std::string& name) const
		{
			LI_CORE_ASSERT(m_Fonts.find(name) != m_Fonts.end(), "Font not found.");
			return m_Fonts.at(name);
		}

		template<>
		Ref<Audio> GetImpl<Audio>(const std::string& name) const
		{
			LI_CORE_ASSERT(m_Audio.find(name) != m_Audio.end(), "Audio not found.");
			return m_Audio.at(name);
		}

		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		std::unordered_map<std::string, Ref<TextureAtlas>> m_TextureAtlases;
		std::unordered_map<std::string, Ref<Font>> m_Fonts;
		std::unordered_map<std::string, Ref<Audio>> m_Audio;

		static Scope<ResourceManager> s_Instance;
	};
}