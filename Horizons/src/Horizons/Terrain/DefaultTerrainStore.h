#pragma once

#include "TerrainStore.h"

#include "Lithium.h"
#include "glm/glm.hpp"

#include <array>
#include <fstream>
#include <atomic>
#include <mutex>

class DefaultTerrainStore : public TerrainStore
{
public:
	DefaultTerrainStore(int store_width);
	virtual ~DefaultTerrainStore();
	virtual bool LoadTerrain(const std::string& path, glm::ivec2 center) override;
	virtual void UnloadTerrain() override;
	virtual void UpdateCenter(glm::ivec2 center) override;
	virtual void LoadRenderChunkData(glm::ivec2 store_coord, RenderChunk* destination) override;

	virtual glm::ivec2 GetWorldSize() const override { return { m_WorldWidth, m_WorldHeight }; }
	virtual bool ReloadRenderChunks() override { return false; }

private:
	struct StoreChunk
	{
		glm::ivec2 Coord;
		glm::ivec2 CenterOffset;

		uint16_t Tiles[Terrain::NumTilesPerChunk];
		AlphaValuesArray AlphaValues;

		std::mutex Mutex;
	};

	void LoadInitialChunks(glm::ivec2 center);
	void LoadThreadEntryPoint(glm::ivec2 initial_center);
	void LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination);

	bool m_Open;
	std::string m_TerrainPath;
	std::ifstream m_TerrainFile;

	const int m_StoreWidth;
	int m_WorldWidth;
	int m_WorldHeight;

	std::mutex m_CenterMutex;
	glm::ivec2 m_Center;

	StoreChunk* m_StoreChunks;
	std::atomic<bool> m_RunLoadThread;
	std::thread m_LoadThread;
};