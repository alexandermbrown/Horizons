#pragma once
#ifndef LI_DIST

#include "entt/entt.hpp"
#include "glm/glm.hpp"

namespace ChunkBorderSystem
{
	void SubmitBorders(entt::registry& registry, glm::vec2 camera_focus, glm::ivec2 world_size);
}
#endif
