#pragma once

#include "Horizons/Gameplay/Sync/Sync.h"

#include "Lithium.h"
#include "entt/entt.hpp"
#include "SDL.h"

namespace PlayerSystem
{
	void Init(entt::registry& registry, SyncEventQueue* queue);

	void Update(entt::registry& registry, li::duration::us dt);
	void OnEvent(entt::registry& registry, SDL_Event* event);
};
