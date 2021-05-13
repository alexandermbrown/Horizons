#pragma once
#ifndef LI_SI_VIEW_PROJ_CB_H
#define LI_SI_VIEW_PROJ_CB_H
#include "ShaderInterop.h"

#ifdef __cplusplus
namespace Li {
#endif

LI_CBUFFER(ViewProjCB, 0)
{
	float4x4 u_ViewProj;
};

#ifdef __cplusplus
}
#endif

#endif // LI_SI_VIEW_PROJ_CB_H
