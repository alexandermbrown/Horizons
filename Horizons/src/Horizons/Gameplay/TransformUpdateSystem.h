#pragma once

#include "Components.h"
#include "entt/entt.hpp"

class TransformUpdateSystem
{
public:

	static void Update(entt::registry& registry);
};