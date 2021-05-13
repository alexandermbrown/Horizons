#pragma once
#ifndef LI_SHADERINTEROP_H
#define LI_SHADERINTEROP_H

#ifdef __cplusplus
#include "glm/glm.hpp"
#include <stdint.h>

// Inspired by WickedEngine.

#ifdef LI_PLATFORM_WINDOWS
#define LI_ALIGN_16 __declspec(align(16))
#else
#error Test attribute alignment on other platforms.
#define LI_ALIGN_16 __attribute__((aligned(16)))
#endif

typedef LI_ALIGN_16 uint32_t uint;
typedef LI_ALIGN_16 glm::vec3 float3;
typedef LI_ALIGN_16 glm::vec4 float4;
typedef LI_ALIGN_16 glm::mat4 float4x4;

#define LI_CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define LI_CBUFFER(name, slot) static const int LI_CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#else
#define LI_CBUFFER(name, slot) cbuffer name : register(b ## slot)

#endif // __cplusplus

#endif // LI_SHADERINTEROP_H
