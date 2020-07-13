#pragma once

#include "SyncTransform.h"

#include "entt/entt.hpp"

class SyncTransformSendSystem
{
public:

	static void Update(entt::registry& registry, SyncTransformQueue* queue);
};