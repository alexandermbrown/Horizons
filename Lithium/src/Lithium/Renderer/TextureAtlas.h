#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace li
{
	class TextureAtlas
	{
	public:
		TextureAtlas(const std::string& textureAlias, const std::initializer_list<std::pair<const std::string, glm::vec4>>& entries);
		TextureAtlas(const std::string& textureAlias, const std::unordered_map<std::string, glm::vec4>& entries);

		TextureAtlas(Ref<Texture2D> texture, const std::initializer_list<std::pair<const std::string, glm::vec4>>& entries);
		TextureAtlas(Ref<Texture2D> texture, const std::unordered_map<std::string, glm::vec4>& entries);

		void Bind();
		inline const glm::vec4& GetBounds(const std::string& textureAlias) const { return m_Entries.at(textureAlias); }
		inline const std::unordered_map<std::string, glm::vec4>& GetEntries() const { return m_Entries; }

	private:

		std::string m_TextureAlias;
		Ref<Texture2D> m_Texture;
		std::unordered_map<std::string, glm::vec4> m_Entries;
	};
}