#include "pch.h"
#ifndef LI_DIST
#include "TerrainVerticeSystem.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Terrain/TerrainRenderer.h"
#include "Horizons/Core/Math.h"
#include "EditorComponents.h"

#include "Lithium.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

void TerrainVerticeSystem::SubmitVerticesShow(entt::registry& registry, glm::ivec2 chunk_center, int render_width)
{
	auto& camera = registry.ctx<cp::camera>();
	float vertex_size = std::min(camera.current_zoom * 0.0075f, 0.5f);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { vertex_size / 2.0f, vertex_size, 1.0f });
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), (float)M_PI / 4.0f, { 0.0f, 0.0f, 1.0f });
	constexpr float distance_x = TerrainRenderer::MetersPerChunk / (float)TerrainRenderer::ChunkWidth;
	constexpr float distance_y = TerrainRenderer::MetersPerChunk / (float)TerrainRenderer::ChunkHeight;

	int half_render_width = render_width / 2;
	for (int chunk_dy = -half_render_width; chunk_dy <= half_render_width; chunk_dy++)
	{
		for (int chunk_dx = -half_render_width; chunk_dx <= half_render_width; chunk_dx++)
		{
			glm::ivec2 chunk_pos = chunk_center + glm::ivec2{ chunk_dx, chunk_dy };
			
			for (int y = 0; y < TerrainRenderer::ChunkHeight; y++)
			{
				for (int x = 0; x < TerrainRenderer::ChunkWidth; x++)
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), {
						chunk_pos.x * TerrainRenderer::MetersPerChunk + x * distance_x,
						chunk_pos.y * TerrainRenderer::MetersPerChunk + y * distance_y,
						0.0f });
					li::Renderer::SubmitColored({ 1.0f, 0.1f, 0.1f, 1.0f }, transform * scale * rotation);
				}
			}
		}
	}
}

void TerrainVerticeSystem::SubmitVerticesShowInBrush(entt::registry& registry, glm::ivec2 chunk_center, int render_width)
{
	// TODO: iterate through vertices under brush instead of chunks then vertices.

	auto& brush = registry.ctx<cp::brush>();
	auto& camera = registry.ctx<cp::camera>();
	float vertex_size = std::min(camera.current_zoom * 0.0075f, 0.5f);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { vertex_size / 2.0f, vertex_size, 1.0f });
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), (float)M_PI / 4.0f, { 0.0f, 0.0f, 1.0f });
	
	const float brush_left = brush.world_pos.x - brush.brush->OuterRadius;
	const float brush_right = brush.world_pos.x + brush.brush->OuterRadius;

	const float brush_bottom = brush.world_pos.y - brush.brush->OuterRadius;
	const float brush_top = brush.world_pos.y + brush.brush->OuterRadius;

	int half_render_width = render_width / 2;
	for (int chunk_dy = -half_render_width; chunk_dy <= half_render_width; chunk_dy++)
	{
		for (int chunk_dx = -half_render_width; chunk_dx <= half_render_width; chunk_dx++)
		{
			glm::ivec2 chunk_pos = chunk_center + glm::ivec2{ chunk_dx, chunk_dy };

			const float chunk_x = chunk_pos.x * TerrainRenderer::MetersPerChunk;
			const float chunk_y = chunk_pos.y * TerrainRenderer::MetersPerChunk;

			bool in_x = brush_right >= chunk_x && brush_left <= chunk_x + TerrainRenderer::MetersPerChunk;
			bool in_y = brush_top >= chunk_y && brush_bottom <= chunk_y + TerrainRenderer::MetersPerChunk;
			if (in_x && in_y)
			{
				for (int y = 0; y < TerrainRenderer::ChunkHeight; y++)
				{
					for (int x = 0; x < TerrainRenderer::ChunkWidth; x++)
					{
						constexpr float distance_x = TerrainRenderer::MetersPerChunk / (float)TerrainRenderer::ChunkWidth;
						constexpr float distance_y = TerrainRenderer::MetersPerChunk / (float)TerrainRenderer::ChunkHeight;
						glm::vec2 vert = {
							chunk_pos.x * TerrainRenderer::MetersPerChunk + x * distance_x,
							chunk_pos.y * TerrainRenderer::MetersPerChunk + y * distance_y
						};
						float distance = glm::distance(vert, brush.world_pos);
						float alpha = 1.0f - std::clamp((distance - brush.brush->InnerRadius) / (brush.brush->OuterRadius - brush.brush->InnerRadius), 0.0f, 1.0f);

						glm::mat4 transform = glm::translate(glm::mat4(1.0f), { vert, 0.0f });
						li::Renderer::SubmitColored({ 1.0f, 0.1f, 0.1f, alpha }, transform * scale * rotation);
					}
				}
			}
		}
	}
}
#endif
