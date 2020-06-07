#pragma once

#include "entt/entt.hpp"

namespace MovementSystem
{
	void Init(entt::registry& registry);
	void Shutdown(entt::registry& registry);

	void Update(entt::registry& registry, float dt);
}