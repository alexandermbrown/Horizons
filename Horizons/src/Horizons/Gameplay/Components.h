#pragma once

#include "glm/glm.hpp"
#include "entt/entt.hpp"

#include <unordered_map>

namespace cp
{
	struct transform
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		float rotation = 0.0f;
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		bool old = false;
		glm::mat4 transform;
	};
}