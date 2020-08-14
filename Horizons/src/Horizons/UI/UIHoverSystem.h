#pragma once

#include "entt/entt.hpp"
#include "glm/glm.hpp"

class UIHoverSystem
{
public:
	static void OnMouseMove(entt::registry& registry, int x, int y);
};