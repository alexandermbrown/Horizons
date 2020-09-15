#pragma once

#include "Lithium.h"
#include "glm/glm.hpp"
#include <string>

namespace Terrain
{
	static constexpr int NumTilesPerChunk = 4;
	static constexpr int ChunkWidth = 8;
	static constexpr int ChunkHeight = 8;
}

struct RenderChunk
{
	glm::ivec2 Coord;
	glm::ivec2 CenterOffset;

	uint16_t Tiles[Terrain::NumTilesPerChunk];

	glm::mat4 Transform;

	li::Ref<li::VertexBuffer> AlphaVB;
	li::Ref<li::VertexArray> VertexArray;
};

class TerrainStore
{
public:
	virtual ~TerrainStore() = default;
	virtual bool LoadTerrain(const std::string& path, glm::ivec2 center) = 0;
	virtual void UnloadTerrain() = 0;
	virtual void UpdateCenter(glm::ivec2 center) = 0;
	virtual void LoadRenderChunkData(glm::ivec2 store_coord, RenderChunk* destination) = 0;

	virtual int GetWorldWidth() const = 0;
	virtual int GetWorldHeight() const = 0;

	static constexpr int NumTilesPerChunk = Terrain::NumTilesPerChunk;
	static constexpr int ChunkWidth = Terrain::ChunkWidth;
	static constexpr int ChunkHeight = Terrain::ChunkHeight;

	using AlphaValuesArray = float[ChunkHeight + 1][ChunkWidth + 1][NumTilesPerChunk - 1];

protected:
	struct TerrainFileHeader
	{
		uint16_t WorldWidth;
		uint16_t WorldHeight;
	};

	struct TerrainFileChunk
	{
		uint16_t Tiles[NumTilesPerChunk];
		uint8_t AlphaValues[ChunkHeight][ChunkWidth][NumTilesPerChunk - 1];
	};
};

