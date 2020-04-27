#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Texture.h"
#include "Lithium/Renderer/Shader.h"
#include "Lithium/Renderer/TextureAtlas.h"
#include "Lithium/Renderer/Font.h"

#include <unordered_map>

namespace li
{
	class ResourceManager
	{
	public:
		static void Init(const std::string& labFilePath) {
			s_Instance->InitImpl(labFilePath);
		}

		template<typename T>
		static Ref<T> Get(const std::string& name)
		{
			return s_Instance->GetImpl<T>(name);
		}

		ResourceManager();

	private:
		void InitImpl(const std::string& labFilePath);

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

		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		std::unordered_map<std::string, Ref<TextureAtlas>> m_TextureAtlases;
		std::unordered_map<std::string, Ref<Font>> m_Fonts;

		static Scope<ResourceManager> s_Instance;
	};
}