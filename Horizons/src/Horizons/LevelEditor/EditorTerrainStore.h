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

	virtual glm::ivec2 GetWorldSize() const override { return { m_WorldWidth, m_WorldHeight }; }

	void ApplyBrush(BrushSettings* brush, glm::vec2 brush_pos, int layer, float dt);
	void Save();
	bool SaveAs(const std::string& path);

	inline bool IsModified() const { return m_Modified; }
	virtual bool ReloadRenderChunks() override;

	static bool CreateEmptyTerrainFile(const std::string& path, glm::ivec2 world_size);

private:
	struct StoreChunk
	{
		glm::ivec2 Coord;
		bool Modified;
		uint16_t Tiles[Terrain::NumTilesPerChunk];
		AlphaValuesArray AlphaValues;
	};

	void LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination);
	void ApplyBrushToChunk(BrushSettings* brush, glm::vec2 brush_pos, int layer, float dt, StoreChunk& chunk);
	void OverwriteWorld();

	// Assumes we are already in the correct file position.
	void SaveChunk(StoreChunk& chunk);

	bool m_Open;
	bool m_Modified;
	std::string m_TerrainPath;
	std::fstream m_TerrainFile;

	int m_WorldWidth;
	int m_WorldHeight;

	StoreChunk* m_StoreChunks;

	bool m_ReloadRenderChunks;
};
#endif
