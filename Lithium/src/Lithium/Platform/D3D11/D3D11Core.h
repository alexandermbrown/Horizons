#pragma once

#include "Lithium/Core/Core.h"

#ifdef LI_ENABLE_ASSERTS
#	define D3D11Call(x) LI_CORE_VERIFY(SUCCEEDED(x), "D3D11 error")

#else
#	define D3D11Call(x) x
#endif

#define LI_D3D_RELEASE(x) if (x) { x->Release(); x = nullptr; }