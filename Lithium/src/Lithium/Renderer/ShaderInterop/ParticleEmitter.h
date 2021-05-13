#pragma once
#ifndef LI_SI_PARTICLE_EMITTER_H
#define LI_SI_PARTICLE_EMITTER_H

#include "ShaderInterop.h"

// Particle system inspired by WickedEngine.

struct Particle
{
	float3 position;
	float3 velocity;
};

LI_CBUFFER(EmitterCB, 3)
{
	float4x4 u_EmitterTransform;
	uint u_EmitterCount;
};

static const uint PARTICLECOUNTER_OFFSET_ALIVECOUNT = 0;
static const uint PARTICLECOUNTER_OFFSET_DEADCOUNT = PARTICLECOUNTER_OFFSET_ALIVECOUNT + 4;
static const uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT = PARTICLECOUNTER_OFFSET_DEADCOUNT + 4;
static const uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION = PARTICLECOUNTER_OFFSET_REALEMITCOUNT + 4;

static const uint THREADCOUNT_EMIT = 256;
static const uint THREADCOUNT_SIMULATION = 256;

#endif // LI_SI_PARTICLE_EMITTER_H
