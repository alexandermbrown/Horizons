#pragma once

#include "Lithium.h"
#include "glm/glm.hpp"

#include "TerrainStore.h"

#include <array>

class TerrainRenderer
{
public:
	TerrainRenderer(TerrainStore* store, int render_width);
	~TerrainRenderer();

	TerrainRenderer() = delete;
	TerrainRenderer(const TerrainRenderer&) = delete;
	TerrainRenderer(TerrainRenderer&&) = delete;
	TerrainRenderer& operator=(const TerrainRenderer&) = delete;

	bool LoadTerrain(const std::string& path, glm::ivec2 center);
	void UnloadTerrain();
	void UpdateCenter(glm::ivec2 center);

	void RenderFramebuffer();
	void SubmitQuad();
	inline glm::ivec2 GetCenter() const { return m_Center; }

	static constexpr float MetersPerChunk = 16.0f;

	static constexpr int ChunkWidth = Terrain::ChunkWidth;
	static constexpr int ChunkHeight = Terrain::ChunkHeight;
	static constexpr int NumTilesPerChunk = Terrain::NumTilesPerChunk;

	static constexpr int ChunkWidthInPixels = 512;
	static constexpr int ChunkHeightInPixels = 256;

	const int RenderWidth;
private:
	void PrepareRenderChunks();
	
	glm::ivec2 m_Center;
	glm::ivec2 m_PrevCenter;

	RenderChunk* m_RenderChunks;
	bool m_ReloadRenderChunks;
	bool m_RenderCenterChanged;

	Li::Ref<Li::Shader> m_TerrainShader;
	Li::Ref<Li::UniformBuffer> m_AtlasBoundsUB;
	Li::Ref<Li::Framebuffer> m_Framebuffer;

	glm::mat4 m_QuadTransform;
	Li::Unique<Li::OrthographicCamera> m_TerrainCamera;

	TerrainStore* m_Store;
};