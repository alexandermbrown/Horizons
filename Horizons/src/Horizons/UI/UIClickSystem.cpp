#include "pch.h"
#include "UIClickSystem.h"

#include "UIComponents.h"
#include "Horizons/Core/Math.h"

#include "Lithium.h"
#include <limits>

bool OnMouseDown(entt::registry& registry, int x, int y, int button);
bool OnMouseUp(entt::registry& registry, int x, int y, int button);

void Systems::UIClick::OnEvent(entt::registry& registry, SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		OnMouseDown(registry, event->button.x, event->button.y, event->button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		OnMouseUp(registry, event->button.x, event->button.y, event->button.button);
		break;
	}
}

bool OnMouseDown(entt::registry& registry, int x, int y, int button)
{
	auto view = registry.view<cp::ui_transform>();

	bool clickHandled = false;
	bool mouseDownHandled = false;

#ifdef LI_ENABLE_ASSERTS
	float prev = std::numeric_limits<float>::max();
#endif

	for (auto it = view.rbegin(); it != view.rend(); ++it)
	{
		entt::entity entity = *it;
		if (!registry.has<cp::ui_click>(entity))
			continue;
		auto& click = registry.get<cp::ui_click>(entity);
		auto& transform = view.get<cp::ui_transform>(entity);

#ifdef LI_ENABLE_ASSERTS
		LI_ASSERT(prev >= transform.transform[3][2], "Must iterate from highest to lowest!");
		prev = transform.transform[3][2];
#endif
		
		if (Math::TransformContainsPoint(transform.transform, x, y))
		{
			// If the user clicks two buttons at once.
			if (click.mouse_button != button && click.mouse_button > 0)
			{
				if (click.start_click && !clickHandled)
				{
					if (click.OnClickFn)
						clickHandled = click.OnClickFn(registry, entity, click.mouse_button);
					else if (click.OnClickLuaFn)
					{
						sol::protected_function_result result = click.OnClickLuaFn(sol::light(registry), entity, click.mouse_button);
						if (result.valid() && result.get_type() == sol::type::boolean)
							clickHandled = result.get<bool>();
						else Li::Log::Error("Bad return from OnClick. Expected bool.");
					}
				}
			}
			click.mouse_button = button;
			click.start_click = true;

			if (click.OnMouseDownFn)
			{
				mouseDownHandled = click.OnMouseDownFn(registry, entity, button);
				if (mouseDownHandled)
					return true;
			}
			else if (click.OnMouseDownLuaFn)
			{
				sol::protected_function_result result = click.OnMouseDownLuaFn(sol::light(registry), entity, click.mouse_button);
				if (result.valid() && result.get_type() == sol::type::boolean)
					mouseDownHandled = result.get<bool>();
				else Li::Log::Error("Bad return from OnMouseDown. Expected bool.");

				if (mouseDownHandled)
					return true;
			}
		}
	}
	return mouseDownHandled;
}

bool OnMouseUp(entt::registry& registry, int x, int y, int button)
{
	auto view = registry.view<cp::ui_transform>();

	bool clickHandled = false;
	bool mouseUpHandled = false;

	for (auto it = view.rbegin(); it != view.rend(); ++it)
	{
		entt::entity entity = *it;
		if (!registry.has<cp::ui_click>(entity))
			continue;
		auto& click = registry.get<cp::ui_click>(entity);
		auto& transform = view.get<cp::ui_transform>(entity);

		if ((!clickHandled || !mouseUpHandled) && Math::TransformContainsPoint(transform.transform, x, y))
		{
			// Only run the click event if it has not been handled by an above element.
			if (click.start_click && (click.mouse_button == button || click.mouse_button == -1) && !clickHandled)
			{
				if (click.OnClickFn)
					clickHandled = click.OnClickFn(registry, entity, button);
				else if (click.OnClickLuaFn)
				{
					sol::protected_function_result result = click.OnClickLuaFn(sol::light(registry), entity, click.mouse_button);
					if (result.valid() && result.get_type() == sol::type::boolean)
						clickHandled = result.get<bool>();
					else Li::Log::Error("Bad return from OnClick. Expected bool.");
				}
			}

			// Only run the mouse up event if it has not been handled by an above element.
			if (!mouseUpHandled)
			{
				if (click.OnMouseUpFn)
					mouseUpHandled = click.OnMouseUpFn(registry, entity, button);
				else if (click.OnMouseUpLuaFn)
				{
					sol::protected_function_result result = click.OnMouseUpLuaFn(sol::light(registry), entity, click.mouse_button);
					if (result.valid() && result.get_type() == sol::type::boolean)
						mouseUpHandled = result.get<bool>();
					else Li::Log::Error("Bad return from OnMouseUp. Expected bool.");
				}
			}
		}
		click.mouse_button = -1;
		click.start_click = false;
	}
	return clickHandled || mouseUpHandled;
}
