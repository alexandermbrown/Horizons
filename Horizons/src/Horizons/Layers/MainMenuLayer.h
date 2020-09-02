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
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	entt::registry m_Registry;
};