#pragma once
#ifndef LI_DIST
#include "entt/entt.hpp"
#include "glm/glm.hpp"

namespace TerrainVerticeSystem
{
	void SubmitVerticesShow(entt::registry& registry, glm::ivec2 chunk_center, int render_width);
	void SubmitVerticesShowInBrush(entt::registry& registry, glm::ivec2 chunk_center, int render_width);
}
#endif