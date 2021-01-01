#include "pch.h"
#include "UIRenderSystem.h"

#include "Lithium.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "UIComponents.h"

void RenderQuads(entt::registry& registry);
void RenderLabels(entt::registry& registry);

void Systems::UIRender::OnRender(entt::registry& registry)
{
	RenderQuads(registry);
	RenderLabels(registry);
}

void RenderQuads(entt::registry& registry)
{
	Li::Renderer::BeginUI();
	auto ui_transform_view = registry.view<cp::ui_transform>(entt::exclude<cp::label>);
	for (entt::entity entity : ui_transform_view)
	{
		auto& transform = ui_transform_view.get<cp::ui_transform>(entity);

		bool has_color = registry.has<cp::color>(entity);
		bool has_texture = registry.has<cp::texture>(entity);

		if (has_color && has_texture)
		{
			Li::Renderer::UISubmitColoredTexture(registry.get<cp::texture>(entity).alias, registry.get<cp::color>(entity).color,
				transform.transform, registry.has<cp::ui_texture_crop>(entity));
		}
		else if (has_texture)
		{
			Li::Renderer::UISubmitTextured(registry.get<cp::texture>(entity).alias,
				transform.transform, registry.has<cp::ui_texture_crop>(entity));
		}
		else if (has_color)
		{
			Li::Renderer::UISubmitColored(registry.get<cp::color>(entity).color, transform.transform);
		}
	}
	Li::Renderer::EndUI();
}

void RenderLabels(entt::registry& registry)
{
	registry.view<cp::ui_transform, cp::label>().each([&registry](entt::entity entity, auto& transform, auto& label)
	{
		if (registry.has<cp::color>(entity))
		{
			Li::Renderer::UISubmitLabel(registry.get<cp::label>(entity).label_ref, transform.transform, registry.get<cp::color>(entity).color);
		}
		else
		{
			Li::Renderer::UISubmitLabel(registry.get<cp::label>(entity).label_ref, transform.transform, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	});
}
