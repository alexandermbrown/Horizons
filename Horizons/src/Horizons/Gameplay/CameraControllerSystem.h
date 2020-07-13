#pragma once

#include "entt/entt.hpp"
#include "SDL.h"

class CameraControllerSystem
{
public:
	static void Init(entt::registry& registry);
	static void Shutdown(entt::registry& registry);

	static void Update(entt::registry& registry, float dt);

	static void OnEvent(entt::registry& registry, SDL_Event* event);
};