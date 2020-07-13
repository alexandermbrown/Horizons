#pragma once

#include "Horizons/Core/Core.h"
#include "Horizons/Core/ConfigStore.h"
#include "Horizons/Gameplay/Sync/Sync.h"
#include "Horizons/Gameplay/Sync/SyncTransform.h"

#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#endif

class Game
{
public:
	static Game* Get() { return s_Instance; }

	Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, SyncEventQueue* syncQueue, SyncTransformQueue* transformQueue, const ConfigStore& config);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, SyncEventQueue* syncQueue, SyncTransformQueue* transformQueue, const ConfigStore& config, DebugDrawCommandQueue* debugDrawQueue);
#endif

	~Game();

	void Run();

	void OnEvent(SDL_Event* event);

	ConfigStore& GetConfig() { return m_ConfigStore; }

private:

	void Init();
	void Update(float dt);
	void Shutdown();

	bool m_Running;
	std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTime;
	moodycamel::ReaderWriterQueue<SDL_Event>* m_EventQueue;
	SyncEventQueue* m_SyncQueue;
	SyncTransformQueue* m_TransformQueue;

	entt::registry m_Registry;

	ConfigStore m_ConfigStore;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue* m_DebugDrawQueue;
#endif

	static Game* s_Instance;
};