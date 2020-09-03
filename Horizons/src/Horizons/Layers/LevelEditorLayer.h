#pragma once

#include "Lithium.h"
#include "entt/entt.hpp"

class LevelEditorLayer : public li::Layer
{
public:
	LevelEditorLayer();
	virtual ~LevelEditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	entt::registry m_Registry;

	float m_BrushInnerRadius;
	float m_BrushOuterRadius;
	float m_BrushAmplitude;
};