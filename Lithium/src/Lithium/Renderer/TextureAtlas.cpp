#include "lipch.h"
#include "TextureAtlas.h"

#include "Lithium/Resources/ResourceManager.h"

namespace Li
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
			m_Texture = ResourceManager::GetTexture2D(m_TextureAlias);
		}
		m_Texture->Bind(slot);
	}

	const glm::vec4& TextureAtlas::GetBounds(const std::string& texture_alias) const
	{
		auto iter = m_Entries.find(texture_alias);
		if (iter != m_Entries.end())
			return iter->second;

		Li::Log::Error("Alias {} not found in texture atlas with texture '{}'", texture_alias, m_TextureAlias);
		// Ensure we don't return a dangling pointer.
		static const glm::vec4 default_bounds = { 0.0f, 0.0f, 1.0f, 1.0f };
		return default_bounds;
	}
}
