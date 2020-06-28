#include "lipch.h"
#include "TextureAtlasLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	TextureAtlasArgs* TextureAtlasArgs::Deserialize(zstr::ifstream* inFile, size_t* pos)
	{
		char name[64];
		char texture[64];
		uint32_t numEntries;
		std::unordered_map<std::string, glm::vec4> entries;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);
		LI_READ_FILE(*inFile, (char*)&texture, sizeof(texture), *pos);
		LI_READ_FILE(*inFile, (char*)&numEntries, sizeof(numEntries), *pos);

		for (uint32_t i = 0; i < numEntries; i++)
		{
			glm::vec4 bounds;
			char alias[64];
			LI_READ_FILE(*inFile, alias, sizeof(alias), *pos);
			LI_READ_FILE(*inFile, (char*)&bounds, sizeof(bounds), *pos);

			entries[alias] = bounds;
		}

		return new TextureAtlasArgs(name, texture, std::move(entries));
	}
}