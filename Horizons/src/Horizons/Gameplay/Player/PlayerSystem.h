#pragma once

#include "entt/entt.hpp"
#include "SDL.h"

#include "Horizons/Gameplay/Sync/Sync.h"

namespace PlayerSystem
{
	void Init(entt::registry& registry, SyncEventQueue* queue);

	void Update(entt::registry& registry, float dt);
	void OnEvent(entt::registry& registry, SDL_Event* event);
};