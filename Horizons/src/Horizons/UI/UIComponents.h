#pragma once

#include "layout/layout.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"

namespace cp
{
	struct ui_transform
	{
		float z = 0.0f;
		glm::mat4 transform;
	};

	struct ui_context
	{
		lay_context context;
		bool rebuild;
		bool recalculate;
	};

	struct ui_element
	{
		entt::entity first_child{ entt::null };
		entt::entity next_sibling{ entt::null };
		entt::entity parent{ entt::null };


		lay_id layout_id = LAY_INVALID_ID;

		lay_scalar width = 0;
		lay_scalar height = 0;
		
		// Order: left, top, right, bottom.
		lay_vec4 margins{ 0, 0, 0, 0 };

		uint32_t layout_behave = 0;
		uint32_t layout_contain = 0;
	};

	struct ui_button
	{

	};
}

