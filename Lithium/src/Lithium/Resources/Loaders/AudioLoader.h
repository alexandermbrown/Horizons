#pragma once

#include "Lithium/Audio/Audio.h"
#include "zstr/zstr.hpp"

namespace li
{
	Ref<Audio> LoadAudio(std::string* outname, zstr::ifstream* inFile, size_t* pos);
}