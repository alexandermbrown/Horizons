#pragma once

#include "Lithium.h"
#include "entt/entt.hpp"

namespace FlickerSystem
{
	void Update(entt::registry& registry, li::duration::us dt);
}