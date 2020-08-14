#include "pch.h"
#include "UIRenderSystem.h"

#include "Lithium.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "UIComponents.h"

void UIRenderSystem::Render(entt::registry& registry)
{
	auto ui_transform_view = registry.view<cp::ui_transform>();
	for (entt::entity entity : ui_transform_view)
	{
		auto& transform = ui_transform_view.get<cp::ui_transform>(entity);

		bool has_color = registry.has<cp::color>(entity);
		bool has_texture = registry.has<cp::texture>(entity);
		bool has_label = registry.has<cp::label>(entity);

		if (has_color && has_texture)
		{
			LI_ASSERT(!has_label, "Cannot render both texture and label!");
			li::Renderer::UISubmitColoredTexture(registry.get<cp::texture>(entity).alias, registry.get<cp::color>(entity).color, transform.transform);
		}
		else if (has_color && has_label)
		{
			li::Renderer::UISubmitLabel(registry.get<cp::label>(entity).label_ref, transform.transform, registry.get<cp::color>(entity).color);
		}
		else if (has_color)
		{
			li::Renderer::UISubmitColored(registry.get<cp::color>(entity).color, transform.transform);
		}
		else if (has_texture)
		{
			li::Renderer::UISubmitTextured(registry.get<cp::texture>(entity).alias, transform.transform);
		}
		else if (has_label)
		{
			li::Renderer::UISubmitLabel(registry.get<cp::label>(entity).label_ref, transform.transform, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}
}
