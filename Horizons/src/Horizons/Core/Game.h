#pragma once

#include "Horizons/Config/ConfigStore.h"
#include "Horizons/Gameplay/Sync/Sync.h"
#include "Horizons/Gameplay/Sync/SyncTransform.h"

#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#endif

#include "TickThread.h"

class Game
{
public:
	static Game* Get() { return s_Instance; }

	Game(const TickThreadInput& data);
#ifdef HZ_PHYSICS_DEBUG_DRAW
	Game(const TickThreadInput& data, DebugDrawCommandQueue* debug_draw_queue);
#endif
	~Game();

	void Run();
	void OnEvent(SDL_Event* event);

	ConfigStore& GetConfig() { return m_ConfigStore; }

private:
	void Init();
	void Update(Li::Duration::us dt);
	void Deinit();

	bool m_Running;
	std::atomic<bool>* m_AppRun;

	std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTime;
	moodycamel::ReaderWriterQueue<SDL_Event>* m_EventQueue;
	SyncEventQueue* m_SyncQueue;
	SyncTransformQueue* m_TransformQueue;

	entt::registry m_Registry;

	ConfigStore m_ConfigStore;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue* m_DebugDrawQueue = nullptr;
#endif

	static Game* s_Instance;
};
