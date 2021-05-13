#pragma once
#ifndef LI_SI_FONT_CB_H
#define LI_SI_FONT_CB_H
#include "ShaderInterop.h"

#ifdef __cplusplus
namespace Li {
#endif

LI_CBUFFER(FontCB, 2)
{
	float4 u_Color;
	float u_DistanceFactor;
};

#ifdef __cplusplus
}
#endif

#endif // LI_SI_FONT_CB_H
