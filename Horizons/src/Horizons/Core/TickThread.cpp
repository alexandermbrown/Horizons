#include "pch.h"
#include "TickThread.h"

#include "Game.h"
#include "Horizons.h"

#include "Horizons/Gameplay/Sync/SyncEventReceiveSystem.h"
#include "Horizons/Gameplay/Sync/SyncTransformReceiveSystem.h"

TickThread::TickThread()
	: m_EventQueue(256ull), m_SyncQueue(256ull), m_TransformQueue(256ull),
	m_ThreadRun(false), m_Started(false)
#ifdef HZ_PHYSICS_DEBUG_DRAW
	, m_DebugDrawQueue(256ull)
#endif
{
}

void TickThread::Begin(entt::registry& registry)
{
	if (m_Started)
	{
		LI_ERROR("Tick thread already started!");
	}
	else
	{
		m_Started = true;
		SyncEventReceiveSystem::Init(registry);
		SyncTransformReceiveSystem::Init(registry);

		// Start up tick thread.
		m_ThreadRun = true;
		TickThreadInput threadData;
		threadData.Running = &m_ThreadRun;
		threadData.EventQueue = &m_EventQueue;
		threadData.SyncQueue = &m_SyncQueue;
		threadData.TransformQueue = &m_TransformQueue;
		threadData.Config = Li::Application::Get<Horizons>().GetConfig();

#ifdef HZ_PHYSICS_DEBUG_DRAW
		m_TickThread = std::thread(TickThread::ThreadEntryPointDebugDraw, threadData, &m_DebugDrawQueue);
#else
		m_TickThread = std::thread(TickThread::ThreadEntryPoint, threadData);
#endif
	}
}

void TickThread::Finish(entt::registry& registry)
{
	m_ThreadRun = false;
	m_TickThread.join();
	// Clear queue to free pointers.
	SyncEventReceiveSystem::Update(registry, &m_SyncQueue);
	m_Started = false;
}

void TickThread::UpdateSync(entt::registry& registry, Li::Duration::us dt)
{
	SyncEventReceiveSystem::Update(registry, &m_SyncQueue);
	SyncTransformReceiveSystem::Update(registry, &m_TransformQueue, dt);
}

void TickThread::OnEvent(SDL_Event* event)
{
	m_EventQueue.enqueue(*event);
}

int TickThread::ThreadEntryPoint(const TickThreadInput& data)
{
	LI_INFO("Tick thread starting...");

	Game game(data);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThread::ThreadEntryPointDebugDraw(const TickThreadInput& data, DebugDrawCommandQueue* debugDrawQueue)
{
	LI_INFO("Tick thread starting...");

	Game game(data, debugDrawQueue);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}
#endif
