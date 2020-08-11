#include "lipch.h"
#include "TextureAtlasLoader.h"

namespace li
{
	Ref<TextureAtlas> TextureAtlasLoader::Load(const Assets::TextureAtlas* atlas)
	{
		std::unordered_map<std::string, glm::vec4> entries;
		for (const Assets::AtlasEntry* entry : *atlas->entries())
		{
			const Assets::Vec4* bounds = entry->bounds();
			entries[entry->alias()->str()] = { bounds->x(), bounds->y(), bounds->z(), bounds->w() };
		}

		return CreateRef<TextureAtlas>(atlas->texture()->str(), std::move(entries));
	}
}