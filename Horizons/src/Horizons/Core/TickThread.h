#pragma once

#include "Horizons/Core/Core.h"
#include "Horizons/Core/ConfigStore.h"
#include "Horizons/Gameplay/Sync/Sync.h"
#include "Horizons/Gameplay/Sync/SyncTransform.h"

#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"


#ifdef HZ_PHYSICS_DEBUG_DRAW
#	include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#endif

int TickThreadEntryPoint(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, SyncEventQueue* syncQueue, SyncTransformQueue* transformQueue, const ConfigStore& config);

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, SyncEventQueue* syncQueue, SyncTransformQueue* transformQueue, const ConfigStore& config, DebugDrawCommandQueue* debugDrawQueue);
#endif