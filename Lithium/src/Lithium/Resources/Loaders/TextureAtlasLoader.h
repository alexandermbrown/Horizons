#pragma once

#include "Lithium/Renderer/TextureAtlas.h"
#include "zstr/zstr.hpp"

namespace li
{
	Ref<TextureAtlas> LoadTextureAtlas(std::unordered_map<std::string, Ref<Texture2D>>& textures, std::string* outname, zstr::ifstream* inFile, size_t* pos);
}