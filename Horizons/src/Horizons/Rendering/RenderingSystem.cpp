#include "pch.h"
#include "RenderingSystem.h"

#include "RenderingComponents.h"
#include "Horizons/Gameplay/Components.h"


void RenderingSystem::Render(entt::registry& registry)
{
	registry.sort<cp::transform>([](const cp::transform& lhs, const cp::transform& rhs) {
		return lhs.position.z < rhs.position.z;
	});


	auto transform_view = registry.view<cp::transform>();
	for (entt::entity entity : transform_view)
	{
		auto& transform = transform_view.get<cp::transform>(entity);

		if (registry.has<cp::color>(entity) && registry.has<cp::texture>(entity))
		{
			li::Renderer::SubmitColoredTexture(registry.get<cp::texture>(entity).alias, registry.get<cp::color>(entity).color, transform.transform);
		}
		else if (registry.has<cp::color>(entity))
		{
			li::Renderer::SubmitColored(registry.get<cp::color>(entity).color, transform.transform);
		}
		else if (registry.has<cp::texture>(entity))
		{
			li::Renderer::SubmitTextured(registry.get<cp::texture>(entity).alias, transform.transform);
		}
	}
}
