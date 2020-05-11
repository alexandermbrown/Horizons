#pragma once

#include "Lithium/Renderer/Shader.h"
#include "zstr/zstr.hpp"

namespace li
{
	Ref<Shader> LoadShader(std::string* outname, zstr::ifstream* inFile, size_t* pos);
}