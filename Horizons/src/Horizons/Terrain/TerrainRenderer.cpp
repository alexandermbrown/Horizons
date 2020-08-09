#include "pch.h"
#include "TerrainRenderer.h"

li::Scope<TerrainRenderer::RendererData> TerrainRenderer::s_Data;

void TerrainRenderer::Init()
{

	for (int y = 0; y < HZ_CHUNK_HEIGHT + 1; y++)
	{
		for (int x = 0; x < HZ_CHUNK_WIDTH + 1; x++)
		{
			s_Data->Vertices[y][x][0] = (float)x;

			//s_Data->Vertices[y][x][1] = (float)y - 0.5f; TODO
		}
	}

}

void TerrainRenderer::BeginTerrain(li::OrthographicCamera* camera)
{

}

void TerrainRenderer::EndTerrain()
{

}