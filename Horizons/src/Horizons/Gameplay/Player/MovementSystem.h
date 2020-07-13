#pragma once

#include "entt/entt.hpp"

class MovementSystem
{
public:
	//static void Init(entt::registry& registry);
	//static void Shutdown(entt::registry& registry);

	static void Update(entt::registry& registry, float dt);
};