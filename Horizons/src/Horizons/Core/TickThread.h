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

#include <atomic>

struct TickThreadData
{
	std::atomic<bool>* Running;
	moodycamel::ReaderWriterQueue<SDL_Event>* EventQueue;
	SyncEventQueue* SyncQueue;
	SyncTransformQueue* TransformQueue;
	ConfigStore Config;
};

int TickThreadEntryPoint(const TickThreadData& data);

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(const TickThreadData& data, DebugDrawCommandQueue* debugDrawQueue);
#endif