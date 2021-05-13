#pragma once
#ifndef HZ_SI_ATLAS_BOUNDS_CB_H
#define HZ_SI_ATLAS_BOUNDS_CB_H
#include "Lithium/Renderer/ShaderInterop/ShaderInterop.h"

LI_CBUFFER(AtlasBoundsCB, 4)
{
	float4 u_AtlasBounds0;
	float4 u_AtlasBounds1;
	float4 u_AtlasBounds2;
	float4 u_AtlasBounds3;
	float3 u_NoiseWeights;
	float3 u_BlendWidths;
};

#endif // HZ_SI_ATLAS_BOUNDS_CB_H
