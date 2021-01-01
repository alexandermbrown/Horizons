#pragma once

#include "Lithium.h"
#include "entt/entt.hpp"

namespace Systems::UISortTransforms
{
	void OnUpdate(entt::registry& registry, Li::Duration::us dt);
}
