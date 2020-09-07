#pragma once

#include "Horizons/Core/Core.h"

#include "Lithium.h"
#include "entt/entt.hpp"

class MainMenuLayer : public li::Layer
{
public:
	MainMenuLayer();
	virtual ~MainMenuLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

	inline li::Scene* GetTransitionScene() { return m_TransitionScene; }
	inline bool IsFinished() const { return m_Finished; }

private:
	entt::registry m_Registry;
	li::Scene* m_TransitionScene = nullptr;

	bool m_Finished = false;
	bool m_StartedTransition = false;
	li::Timer<float> m_TransitionTimer;
};