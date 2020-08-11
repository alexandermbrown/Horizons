#pragma once

#include "Lithium.h"
#include "glm/glm.hpp"

#include "Terrain.h"

#include <unordered_map>

class TerrainRenderer
{
public:
	static void Init();

	static void BeginTerrain(li::OrthographicCamera* camera);
	static void EndTerrain();


private:

	struct RendererData
	{
		li::OrthographicCamera* Camera;
		li::Ref<li::Shader> Shader;

		li::Ref<li::VertexBuffer> VertexBuffer;
		li::Ref<li::VertexArray> VertexArray;

		float Vertices[HZ_CHUNK_HEIGHT + 1][HZ_CHUNK_WIDTH + 1][4 + HZ_TERRAIN_TEXTURE_SLOT_COUNT - 1];

	};

	static li::Scope<RendererData> s_Data;
};