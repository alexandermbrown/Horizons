#pragma once

#include "Lithium/Renderer/TextureAtlas.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class TextureAtlasArgs : public ResourceArgs
	{
	public:
		TextureAtlasArgs(zstr::ifstream* inFile, size_t* pos);

		virtual ~TextureAtlasArgs() = default;

		inline Ref<TextureAtlas> Create()
		{
			return CreateRef<TextureAtlas>(m_Texture, m_Entries);
		};

	private:

		std::string m_Texture;
		std::unordered_map<std::string, glm::vec4> m_Entries;
	};
}