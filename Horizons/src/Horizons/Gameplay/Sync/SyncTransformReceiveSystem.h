#pragma once

#include "Lithium.h"
#include "SyncTransform.h"

#include "entt/entt.hpp"
#include "SDL.h"

class SyncTransformReceiveSystem
{
public:

	static void Init(entt::registry& registry);
	static void Update(entt::registry& registry, SyncTransformQueue* queue, Li::Duration::us dt);
};