#include "lipch.h"
#ifndef LI_DIST

#include "Lithium/Core/Core.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "examples/imgui_impl_sdl.cpp"

#ifdef LI_INCLUDE_OPENGL
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#endif

#ifdef LI_INCLUDE_D3D11
#include "examples/imgui_impl_dx11.cpp"
#endif

#endif