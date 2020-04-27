#include "lipch.h"
#include "TextureAtlas.h"

namespace li
{
	TextureAtlas::TextureAtlas(Ref<Texture2D> texture, const std::initializer_list<std::pair<const std::string, glm::vec4>>& entries)
		: m_Texture(texture), m_Entries(entries)
	{
	}

	TextureAtlas::TextureAtlas(Ref<Texture2D> texture, const std::unordered_map<std::string, glm::vec4>& entries)
		: m_Texture(texture), m_Entries(entries)
	{
	}
}