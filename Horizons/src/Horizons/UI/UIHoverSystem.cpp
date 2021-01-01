#include "pch.h"
#include "UIHoverSystem.h"

#include "UIComponents.h"
#include "Horizons/Core/Math.h"

void Systems::UIHover::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_MOUSEMOTION)
	{
		registry.view<cp::ui_transform, cp::ui_hover>().each([&registry, event](entt::entity entity, cp::ui_transform& transform, cp::ui_hover& hover)
		{
			if (hover.is_hover)
			{
				// If cursor has left the rectangle.
				if (!Math::TransformContainsPoint(transform.transform, event->motion.x, event->motion.y))
				{
					hover.is_hover = false;
					if (hover.OnMouseLeaveFn)
						hover.OnMouseLeaveFn(registry, entity);
					if (hover.OnMouseLeaveLuaFn)
						hover.OnMouseLeaveLuaFn(sol::light(registry), entity);
				}
			}
			else
			{
				// If cursor has entered the rectangle.
				if (Math::TransformContainsPoint(transform.transform, event->motion.x, event->motion.y))
				{
					hover.is_hover = true;
					if (hover.OnMouseEnterFn)
						hover.OnMouseEnterFn(registry, entity);
					if (hover.OnMouseEnterLuaFn)
						hover.OnMouseEnterLuaFn(sol::light(registry), entity);
				}
			}
		});
	}
}
