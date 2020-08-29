#pragma once

#ifdef LI_DEBUG
#define SOL_ALL_SAFETIES_ON 1
#endif
#include "sol.hpp"

struct PrototypeData
{
	sol::state Lua;
};