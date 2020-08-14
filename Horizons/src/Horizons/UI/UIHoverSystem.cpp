#include "pch.h"
#include "UIHoverSystem.h"

#include "UIComponents.h"
#include "Horizons/Core/Math.h"

void UIHoverSystem::OnMouseMove(entt::registry& registry, int x, int y)
{
	registry.view<cp::ui_transform, cp::ui_hover>().each([&registry, x, y](entt::entity entity, cp::ui_transform& transform, cp::ui_hover& hover)
	{
		if (hover.is_hover)
		{
			// If cursor has left the rectangle.
			if (!Math::TransformContainsPoint(transform.transform, x, y))
			{
				hover.is_hover = false;
				if (hover.OnMouseLeaveFn)
					hover.OnMouseLeaveFn(registry, entity);
			}
		}
		else
		{
			// If cursor has entered the rectangle.
			if (Math::TransformContainsPoint(transform.transform, x, y))
			{
				hover.is_hover = true;
				if (hover.OnMouseEnterFn)
					hover.OnMouseEnterFn(registry, entity);
			}
		}
	});
}