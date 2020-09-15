#pragma once
#ifndef LI_DIST
#include "SDL.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"

namespace EditorCameraSystem
{
	void Init(entt::registry& registry);
	void Shutdown(entt::registry& registry);
	void Update(entt::registry& registry, float dt);
	void OnEvent(entt::registry& registry, SDL_Event* event, glm::ivec2 view_size);
	void Resize(entt::registry& registry, int width, int height);
	glm::vec2 GetCameraFocusPoint(entt::registry& registry);
}

#endif