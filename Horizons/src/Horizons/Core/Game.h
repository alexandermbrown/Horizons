#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#endif

class Game
{
public:

	Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue);
#ifdef HZ_PHYSICS_DEBUG_DRAW
	Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, DebugDrawCommandQueue* debugDrawQueue);

#endif

	void Run();

	void OnEvent(SDL_Event* event);

private:

	bool m_Running;
	std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTime;
	moodycamel::ReaderWriterQueue<SDL_Event>* m_EventQueue;

	entt::registry m_Registry;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue* m_DebugDrawQueue;
#endif
};