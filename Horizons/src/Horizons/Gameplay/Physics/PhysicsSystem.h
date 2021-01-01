#pragma once

#include "Lithium.h"
#include "entt/entt.hpp"

namespace PhysicsSystem
{
	void Init(entt::registry& registry);

	void Step(entt::registry& registry, Li::Duration::us dt);
};
