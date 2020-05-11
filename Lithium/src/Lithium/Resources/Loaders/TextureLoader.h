#pragma once

#include "Lithium/Renderer/Texture.h"
#include "zstr/zstr.hpp"

namespace li
{
	Ref<Texture2D> LoadTexture2D(std::string* outname, zstr::ifstream* inFile, size_t* pos);
}