#pragma once

#include "Lithium.h"
#include "glm/glm.hpp"

#include <array>
#include <fstream>
#include <atomic>
#include <mutex>

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
	TerrainStore();
	void LoadTerrain(const std::string& path, glm::ivec2 center);
	void UnloadTerrain();
	void UpdateCenter(glm::ivec2 center);
	void LoadRenderChunkData(glm::ivec2 store_coord, RenderChunk* destination);

	int GetWorldWidth() const { return m_WorldWidth; }
	int GetWorldHeight() const { return m_WorldHeight; }

	static constexpr int StoreWidth = 11;
	static constexpr int NumTilesPerChunk = Terrain::NumTilesPerChunk;
	static constexpr int ChunkWidth = Terrain::ChunkWidth;
	static constexpr int ChunkHeight = Terrain::ChunkHeight;

	struct StoreChunk
	{
		glm::ivec2 Coord;
		glm::ivec2 CenterOffset;

		uint16_t Tiles[Terrain::NumTilesPerChunk];
		float AlphaValues[ChunkHeight + 1][ChunkWidth + 1][NumTilesPerChunk - 1];

		std::mutex Mutex;
	};

private:
	void LoadInitialChunks(glm::ivec2 center);
	void LoadThreadEntryPoint(glm::ivec2 initial_center);
	void LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination);

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

	bool m_Open;
	std::string m_TerrainPath;
	std::ifstream m_TerrainFile;

	int m_WorldWidth;
	int m_WorldHeight;

	std::mutex m_CenterMutex;
	glm::ivec2 m_Center;

	std::array<StoreChunk, StoreWidth * StoreWidth> m_StoreChunks;
	std::atomic<bool> m_RunLoadThread;
	std::thread m_LoadThread;
};