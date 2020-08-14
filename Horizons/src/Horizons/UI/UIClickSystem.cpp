#include "pch.h"
#include "UIClickSystem.h"

#include "UIComponents.h"
#include "Horizons/Core/Math.h"

#include "Lithium.h"

bool UIClickSystem::OnMouseDown(entt::registry& registry, int x, int y, int button)
{
	auto view = registry.view<cp::ui_transform, cp::ui_click>();

	bool clickHandled = false;
	bool mouseDownHandled = false;

	// Iterate in reverse order, highest z-position to lowest.
	for (entt::entity entity : view)
	{
		auto& click = view.get<cp::ui_click>(entity);
		auto& transform = view.get<cp::ui_transform>(entity);
		
		if (Math::TransformContainsPoint(transform.transform, x, y))
		{
			// If the user clicks two buttons at once.
			if (click.mouse_button != button && click.mouse_button > 0)
			{
				if (click.start_click && click.OnClickFn && !clickHandled)
					clickHandled = click.OnClickFn(registry, entity, click.mouse_button);
			}
			click.mouse_button = button;
			click.start_click = true;

			if (click.OnMouseDownFn)
			{
				mouseDownHandled = click.OnMouseDownFn(registry, entity, button);
				if (mouseDownHandled)
					return true;
			}
		}
	}
	return mouseDownHandled;
}

bool UIClickSystem::OnMouseUp(entt::registry& registry, int x, int y, int button)
{
	auto view = registry.view<cp::ui_transform, cp::ui_click>();

	bool clickHandled = false;
	bool mouseUpHandled = false;
	// Iterate in reverse order, highest z-position to lowest.
	for (entt::entity entity : view)
	{
		auto& click = view.get<cp::ui_click>(entity);
		auto& transform = view.get<cp::ui_transform>(entity);

		if ((!clickHandled || !mouseUpHandled) && Math::TransformContainsPoint(transform.transform, x, y))
		{
			// Only run the click event if it has not been handled by an above element.
			if (click.start_click && (click.mouse_button == button || click.mouse_button == -1) && !clickHandled && click.OnClickFn)
				clickHandled = click.OnClickFn(registry, entity, button);

			// Only run the mouse up event if it has not been handled by an above element.
			if (!mouseUpHandled && click.OnMouseDownFn)
				mouseUpHandled = click.OnMouseUpFn(registry, entity, button);
		}
		click.mouse_button = -1;
		click.start_click = false;
	}
	return clickHandled || mouseUpHandled;
}
