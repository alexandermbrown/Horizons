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
	LI_ASSERT(!m_Started, "Tick thread already started!");
	m_Started = true;
	SyncEventReceiveSystem::Init(registry);
	SyncTransformReceiveSystem::Init(registry);

	// Start up tick thread.
	m_ThreadRun = true;
	TickThreadInput thread_data;
	thread_data.Running = &m_ThreadRun;
	thread_data.EventQueue = &m_EventQueue;
	thread_data.SyncQueue = &m_SyncQueue;
	thread_data.TransformQueue = &m_TransformQueue;
	thread_data.Config = Li::Application::Get<Horizons>().GetConfig();

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_TickThread = std::thread(TickThread::ThreadEntryPointDebugDraw, thread_data, &m_DebugDrawQueue);
#else
	m_TickThread = std::thread(TickThread::ThreadEntryPoint, thread_data);
#endif
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
	Game game(data);
	game.Run();

	return 0;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThread::ThreadEntryPointDebugDraw(const TickThreadInput& data, DebugDrawCommandQueue* debug_draw_queue)
{
	Game game(data, debug_draw_queue);
	game.Run();

	return 0;
}
#endif
