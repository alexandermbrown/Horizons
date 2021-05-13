#include "ParticleEmitter.h"

RWStructuredBuffer<Particle> particles;

[numthreads(THREADCOUNT_EMIT, 1, 1)]
void cs_main(uint3 thread_id : SV_DispatchThreadID)
{

}
