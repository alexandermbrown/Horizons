#pragma once

#include "Lithium/Core/Assert.h"

#ifdef LI_ENABLE_ASSERTS
#	define D3D11Call(x) LI_CORE_VERIFY(SUCCEEDED(x), "D3D11 error")

#else
#	define D3D11Call(x) x
#endif
