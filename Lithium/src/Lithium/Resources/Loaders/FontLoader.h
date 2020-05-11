#pragma once

#include "Lithium/Renderer/Font.h"
#include "zstr/zstr.hpp"

namespace li
{
	Ref<Font> LoadFont(std::string* outname, zstr::ifstream* inFile, size_t* pos);
}