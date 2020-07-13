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

		if (registry.has<cp::color>(entity) && registry.has<cp::texture>(entity))
		{
			li::Renderer::UISubmitColoredTexture(registry.get<cp::texture>(entity).alias, registry.get<cp::color>(entity).color, transform.transform);
		}
		else if (registry.has<cp::color>(entity))
		{
			li::Renderer::UISubmitColored(registry.get<cp::color>(entity).color, transform.transform);
		}
		else if (registry.has<cp::texture>(entity))
		{
			li::Renderer::UISubmitTextured(registry.get<cp::texture>(entity).alias, transform.transform);
		}
	}
}
