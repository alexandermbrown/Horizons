#pragma once

#include "entt/entt.hpp"

class PhysicsSystem
{
public:
	static void Init(entt::registry& registry);
	static void Shutdown(entt::registry& registry);

	static void Step(entt::registry& registry, float dt);
};