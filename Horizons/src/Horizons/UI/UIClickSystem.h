#pragma once

#include "entt/entt.hpp"
#include "SDL.h"

namespace Systems::UIClick
{
	void OnEvent(entt::registry& registry, SDL_Event* event);
}
