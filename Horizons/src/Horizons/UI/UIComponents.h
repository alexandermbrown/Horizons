#pragma once

#include "Lithium.h"
#include "layout/layout.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include "sol.hpp"

#include <functional>

namespace cp
{
	struct ui_transform
	{
		glm::mat4 transform;
	};

	struct ui_context
	{
		Li::Unique<lay_context> context;

		// Setting 'rebuild' to true will rebuild the element tree starting from the context root.
		// You do not need to set 'recalculate' to true, the layout will be recalculated regardless.
		// You do not need to call lay_set_***(&context, layout_id, ...).
		// All previous layout_id values will be invalidated.
		bool rebuild;

		// Setting 'recalculate' to true will simply recalculate the positions of all existing
		// elements, using the layout properties from the last 'rebuild'.
		// Set this to true whenever you edit a layout property of 'cp::ui_element' and call
		// lay_set_***(&context, layout_id, ...).
		bool recalculate;

		// The lowest z coordinate.
		float start_z{ 0.0f };
		float z_range{ 1.0f };

		constexpr static lay_id MaxItemCount = 1024;

		ui_context()
		{
			rebuild = true;
			recalculate = true;

			context = Li::MakeUnique<lay_context>();

			lay_init_context(context.get());
			lay_reserve_items_capacity(context.get(), MaxItemCount);
		}

		ui_context(ui_context&& other)
			: context(std::move(other.context)), rebuild(other.rebuild),
			recalculate(other.recalculate), start_z(other.start_z), z_range(other.z_range)
		{
		}

		~ui_context()
		{
			if (context)
				lay_destroy_context(context.get());
		}

		ui_context& operator= (ui_context&& other) noexcept
		{
			context = std::move(other.context);
			rebuild = other.rebuild;
			recalculate = other.recalculate;
			start_z = other.start_z;
			z_range = other.z_range;
			return *this;
		}
	};

	struct ui_element
	{
		entt::entity first_child{ entt::null };
		entt::entity next_sibling{ entt::null };
		entt::entity parent{ entt::null };
		int num_children{ 0 };

		// This value will likely be changed on a ui rebuild.
		lay_id layout_id{ LAY_INVALID_ID };

		lay_scalar width{ 0 };
		lay_scalar height{ 0 };
		
		// Order: left, top, right, bottom.
		lay_vec4 margins{ 0, 0, 0, 0 };

		uint32_t layout_behave{ 0 };
		uint32_t layout_contain{ 0 };

		// Internal.
		float z{ 0.0f };
	};

	struct ui_hover
	{
		std::function<void(entt::registry&, entt::entity)> OnMouseEnterFn;
		std::function<void(entt::registry&, entt::entity)> OnMouseLeaveFn;

		sol::function OnMouseEnterLuaFn;
		sol::function OnMouseLeaveLuaFn;

		bool is_hover = false;
	};

	struct ui_click
	{
		std::function<bool(entt::registry&, entt::entity, int button)> OnMouseDownFn;
		std::function<bool(entt::registry&, entt::entity, int button)> OnMouseUpFn;
		std::function<bool(entt::registry&, entt::entity, int button)> OnClickFn;

		sol::function OnMouseDownLuaFn;
		sol::function OnMouseUpLuaFn;
		sol::function OnClickLuaFn;

		// Internal.
		bool start_click = false;
		int mouse_button = -1;
	};

	struct ui_texture_crop {};
}
