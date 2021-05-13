#pragma once
#ifndef LI_SI_TRANSFORM_CB_H
#define LI_SI_TRANSFORM_CB_H
#include "ShaderInterop.h"

#ifdef __cplusplus
namespace Li {
#endif

LI_CBUFFER(TransformCB, 1)
{
	float4x4 u_Transform;
};

#ifdef __cplusplus
}
#endif

#endif // LI_SI_TRANSFORM_CB_H
