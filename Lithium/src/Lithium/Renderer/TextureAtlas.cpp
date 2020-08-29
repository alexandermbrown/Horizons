#include "lipch.h"
#include "TextureAtlas.h"

#include "Lithium/Resources/ResourceManager.h"

namespace li
{
	TextureAtlas::TextureAtlas(const std::string& textureAlias, const std::initializer_list<std::pair<const std::string, glm::vec4>>& entries)
		: m_TextureAlias(textureAlias), m_Entries(entries), m_Texture(nullptr) 
	{
	}

	TextureAtlas::TextureAtlas(const std::string& textureAlias, const std::unordered_map<std::string, glm::vec4>& entries)
		: m_TextureAlias(textureAlias), m_Entries(entries), m_Texture(nullptr)
	{
	}

	TextureAtlas::TextureAtlas(const std::string& textureAlias, std::unordered_map<std::string, glm::vec4>&& entries)
		: m_TextureAlias(textureAlias), m_Entries(entries), m_Texture(nullptr)
	{
	}

	TextureAtlas::TextureAtlas(Ref<Texture2D> texture, const std::initializer_list<std::pair<const std::string, glm::vec4>>& entries)
		: m_TextureAlias(), m_Entries(entries), m_Texture(texture) {}

	TextureAtlas::TextureAtlas(Ref<Texture2D> texture, const std::unordered_map<std::string, glm::vec4>& entries)
		: m_TextureAlias(), m_Entries(entries), m_Texture(texture) {}

	void TextureAtlas::Bind(uint32_t slot)
	{
		if (m_Texture == nullptr)
		{
			m_Texture = ResourceManager::Get<Texture2D>(m_TextureAlias);
		}
		m_Texture->Bind(slot);
	}
}