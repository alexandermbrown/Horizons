#pragma once

#include "Horizons/Core/Core.h"
#include "Horizons/Core/TickThread.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#include "Horizons/Rendering/DebugPhysicsRenderer.h"
#endif

#include "Lithium.h"
#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

class GameLayer : public li::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

	inline bool ReturnToMainMenu() const { return m_ReturnToMainMenu; }

private:
	li::Ref<li::AudioSource> m_AudioSource;

	entt::registry m_Registry;

	TickThread m_TickThread;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugPhysicsRenderer m_DebugPhysicsRenderer;
#endif

	bool m_ReturnToMainMenu;
};