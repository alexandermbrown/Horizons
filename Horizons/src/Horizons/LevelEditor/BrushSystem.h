#pragma once

#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include "SDL.h"

namespace BrushSystem
{
	void SubmitBrush(entt::registry& registry, glm::ivec2 mouse_pos, glm::ivec2 view_size);
}