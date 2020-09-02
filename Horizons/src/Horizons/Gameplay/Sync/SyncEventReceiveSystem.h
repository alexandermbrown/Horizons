#pragma once

#include "Sync.h"
#include "entt/entt.hpp"

namespace SyncEventReceiveSystem
{
	void Init(entt::registry& registry);
	void Update(entt::registry& registry, SyncEventQueue* queue);
};