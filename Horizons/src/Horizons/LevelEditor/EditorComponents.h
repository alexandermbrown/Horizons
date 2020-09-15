#pragma once

#include "Brush.h"

namespace cp
{
	struct camera_pan
	{
		int button = 3;
		bool started = false;
		glm::ivec2 start_pos{ 0, 0 };
	};

	struct brush
	{
		glm::vec3 world_pos;
		Brush brush;
	};
}