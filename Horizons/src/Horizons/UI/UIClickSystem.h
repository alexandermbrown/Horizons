#pragma once

#include "entt/entt.hpp"

class UIClickSystem
{
public:
	static bool OnMouseDown(entt::registry& registry, int x, int y, int button);
	static bool OnMouseUp(entt::registry& registry, int x, int y, int button);
};