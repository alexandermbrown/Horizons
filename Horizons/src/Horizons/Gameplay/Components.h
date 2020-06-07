#pragma once

#include "glm/glm.hpp"
#include "Lithium.h"

namespace cp
{
	struct transform
	{
		glm::vec3 position;
		float rotation;
		glm::vec3 scale;

		bool old;
		glm::mat4 transform;
	};

	struct texture
	{
		li::Ref<li::Texture2D> texture;
	};

	struct color
	{
		glm::vec4 color;
	};
}