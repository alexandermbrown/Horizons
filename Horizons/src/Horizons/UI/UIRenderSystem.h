#pragma once

#include "entt/entt.hpp"

class UIRenderSystem
{
public:
	static void Render(entt::registry& registry);
	static void RenderLabels(entt::registry& registry);
};