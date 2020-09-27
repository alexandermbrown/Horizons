#include "pch.h"
#ifndef LI_DIST
#include "ChunkBorderSystem.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Terrain/TerrainRenderer.h"

#include "Lithium.h"

void ChunkBorderSystem::SubmitBorders(entt::registry& registry, glm::vec2 camera_focus, glm::ivec2 world_size)
{
	auto& camera = registry.ctx<cp::camera>();

	glm::vec2 camera_span = {
		camera.aspect_ratio * camera.current_zoom,
		camera.current_zoom * 2.0f
	};

	glm::vec2 half_camera_span = camera_span / 2.0f;
	glm::vec2 bottom_left = camera_focus - half_camera_span;
	glm::vec2 top_right = camera_focus + half_camera_span;

	glm::vec2 lines_bottom_left = { std::ceil(bottom_left.x / 16.0f), std::ceil(bottom_left.y / 16.0f) };

	for (float x = lines_bottom_left.x * 16.0f; x <= top_right.x; x += TerrainRenderer::MetersPerChunk)
	{
		glm::vec4 color;
		// Make world border different color.
		if ((int)(x / TerrainRenderer::MetersPerChunk) % world_size.x == 0)
			color = { 0.1f, 0.4f, 1.0f, 0.7f };
		else
			color = { 0.1f, 1.0f, 0.1f, 0.5f };
		li::Renderer::SubmitLine(color, { x, bottom_left.y, 0.0f }, { x, top_right.y, 0.0f });
	}

	for (float y = lines_bottom_left.y * 16.0f; y <= top_right.y; y += TerrainRenderer::MetersPerChunk)
	{
		glm::vec4 color;
		// Make world border different color.
		if ((int)(y / TerrainRenderer::MetersPerChunk) % world_size.y == 0)
			color = { 0.1f, 0.4f, 1.0f, 0.7f };
		else
			color = { 0.1f, 1.0f, 0.1f, 0.5f };
		li::Renderer::SubmitLine(color, { bottom_left.x, y, 0.0f }, { top_right.x, y, 0.0f });
	}
}

#endif
