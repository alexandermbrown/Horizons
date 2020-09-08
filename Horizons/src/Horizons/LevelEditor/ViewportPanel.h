#pragma once

#include "Lithium.h"
#include "SDL.h"
#include "glm/glm.hpp"
#include "entt/entt.hpp"

class ViewportPanel
{
public:
	ViewportPanel();
	~ViewportPanel();

	inline void Open() { m_ViewportOpen = true; }
	void OnUpdate(float dt);
	void OnImGuiRender();
	void OnEvent(SDL_Event* event);

private:
	entt::registry m_Registry;

	bool m_ViewportOpen;
	bool m_ViewportFocused;
	bool m_ViewportHovered;
	glm::ivec2 m_ViewportSize;
	li::Ref<li::Framebuffer> m_ViewportFB;

	float m_BrushInnerRadius;
	float m_BrushOuterRadius;
	float m_BrushAmplitude;
};