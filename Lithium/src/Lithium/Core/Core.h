#pragma once

#define LI_BIT(x) (1 << x)
#define LI_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef LI_PLATFORM_WINDOWS
#define LI_INCLUDE_OPENGL
#define LI_INCLUDE_D3D11
#else
#define LI_INCLUDE_OPENGL
#endif
