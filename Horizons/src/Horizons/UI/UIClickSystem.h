#pragma once

#include "entt/entt.hpp"

namespace UIClickSystem
{
	bool OnMouseDown(entt::registry& registry, int x, int y, int button);
	bool OnMouseUp(entt::registry& registry, int x, int y, int button);
}