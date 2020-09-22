#pragma once
#ifndef LI_DIST
#include "Horizons/Terrain/TerrainStore.h"
#include "EditorSettings.h"

#include "glm/glm.hpp"
#include <fstream>

class EditorTerrainStore : public TerrainStore
{
public:
	EditorTerrainStore();
	virtual ~EditorTerrainStore();
	virtual bool LoadTerrain(const std::string& path, glm::ivec2 center) override;
	virtual void UnloadTerrain() override;
	virtual void UpdateCenter(glm::ivec2 center) override {};
	virtual void LoadRenderChunkData(glm::ivec2 store_coord, RenderChunk* destination) override;

	virtual int GetWorldWidth() const override { return m_WorldWidth; }
	virtual int GetWorldHeight() const override { return m_WorldHeight; }

	void ApplyBrush(BrushSettings* brush, glm::vec2 brush_pos, float dt);

private:
	struct StoreChunk
	{
		glm::ivec2 Coord;
		uint16_t Tiles[Terrain::NumTilesPerChunk];
		AlphaValuesArray AlphaValues;
	};

	void LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination);
	void ApplyBrushToChunk(BrushSettings* brush, glm::vec2 brush_pos, float dt, StoreChunk& chunk);

	bool m_Open;
	std::string m_TerrainPath;
	std::ifstream m_TerrainFile;

	int m_WorldWidth;
	int m_WorldHeight;

	StoreChunk* m_StoreChunks;
};
#endif
