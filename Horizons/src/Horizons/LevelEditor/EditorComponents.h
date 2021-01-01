#pragma once
#ifndef LI_DIST
#include "EditorSettings.h"

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
		BrushSettings* brush;
		glm::vec2 world_pos;
	};
}
#endif
