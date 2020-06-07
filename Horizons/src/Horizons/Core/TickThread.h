#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#	include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#endif

int TickThreadEntryPoint(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue);

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, DebugDrawCommandQueue* debugDrawQueue);
#endif