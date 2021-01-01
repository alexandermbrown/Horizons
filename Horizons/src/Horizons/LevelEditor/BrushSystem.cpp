#include "pch.h"
#ifndef LI_DIST
#include "BrushSystem.h"

#include "EditorComponents.h"
#include "Horizons/Rendering/RenderingComponents.h"

#include "Lithium.h"

void BrushSystem::SubmitBrush(entt::registry& registry, glm::ivec2 mouse_pos, glm::ivec2 view_size)
{
	auto& pan = registry.ctx<cp::camera_pan>();
	auto& brush = registry.ctx<cp::brush>();
	if (!pan.started && brush.brush->Enabled)
	{
		auto& brush = registry.ctx<cp::brush>();
		auto& camera = registry.ctx<cp::camera>();
		const glm::vec3& camera_pos = camera.camera->GetPosition();
		glm::vec2 focus_point = { camera_pos.x, camera_pos.y + 3.0f };

		glm::ivec2 pixel_distance = mouse_pos - glm::ivec2{ view_size.x / 2, view_size.y / 2 };

		brush.world_pos = {
			focus_point.x + (float)pixel_distance.x * camera.current_zoom / (float)view_size.x * camera.aspect_ratio,
			focus_point.y - (float)pixel_distance.y * camera.current_zoom / (float)view_size.y * 2.0f
		};

		Li::Renderer::SubmitCircle({ 1.000f, 0.391f, 0.000f, 1.000f }, { brush.world_pos, 0.0f }, brush.brush->InnerRadius);
		Li::Renderer::SubmitCircle({ 1.000f, 0.391f, 0.000f, 1.000f }, { brush.world_pos, 0.0f }, brush.brush->OuterRadius);
	}
}
#endif