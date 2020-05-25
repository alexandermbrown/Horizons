#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Localization/Localization.h"

#include "zstr/zstr.hpp"

namespace li
{
	Ref<Locale> LoadLocale(std::string* outname, zstr::ifstream* inFile, size_t* pos);
}