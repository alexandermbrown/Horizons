#pragma once

#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#include "Horizons/Rendering/DebugPhysicsRenderer.h"
#endif

#include "Horizons/Gameplay/Sync/Sync.h"
#include "Horizons/Gameplay/Sync/SyncTransform.h"

#include "Lithium.h"
#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

class GameLayer : public li::Layer
{
public:
	GameLayer();
	~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	li::Ref<li::AudioSource> m_AudioSource;

	std::thread m_TickThread;

	moodycamel::ReaderWriterQueue<SDL_Event> m_EventQueue;
	SyncEventQueue m_SyncQueue;
	SyncTransformQueue m_TransformQueue;

	entt::registry m_Registry;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue m_DebugDrawQueue;
	DebugPhysicsRenderer m_DebugPhysicsRenderer;
#endif
};