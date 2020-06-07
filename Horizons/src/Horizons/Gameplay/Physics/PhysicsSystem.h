#pragma once

#include "entt/entt.hpp"

namespace PhysicsSystem
{
	void Init(entt::registry& registry);
	void Shutdown(entt::registry& registry);

	void Step(entt::registry& registry, float dt);
};