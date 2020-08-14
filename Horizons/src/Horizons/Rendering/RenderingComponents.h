#pragma once

#include "glm/glm.hpp"

#include "Lithium.h"

namespace cp
{
	struct texture
	{
		std::string alias;
	};

	struct color
	{
		glm::vec4 color;
	};

	struct label
	{
		li::Ref<li::Label> label_ref;
	};

	struct camera
	{
		li::OrthographicCamera* camera;

		float aspect_ratio = 1.0f;

		float current_zoom = 10.0f;
		float target_zoom = 10.0f;

		float min_zoom = 2.0f;
		float max_zoom = 24.0f;

		bool finished_zoom = false;
	};
}