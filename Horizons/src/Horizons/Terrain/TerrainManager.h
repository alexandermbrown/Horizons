#pragma once

#include "Lithium.h"
#include "glm/glm.hpp"

#include <array>
#include <unordered_map>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>

class TerrainManager
{
public:
	static void Init();
	static void Shutdown();

	static void LoadWorld(const std::string& path, glm::ivec2 center);
	static void UnloadWorld();

	static void UpdateCenter(glm::ivec2 center);

	static void RenderFramebuffer();
	static void RenderQuad();

	static constexpr float MetersPerChunk = 16.0f;

	static constexpr int ChunkWidth = 8;
	static constexpr int ChunkHeight = 8;

	static constexpr int StoreWidth = 11;
	static constexpr int RenderWidth = 3;

	static constexpr int NumTilesPerChunk = 4;

	static constexpr int ChunkWidthInPixels = 512;
	static constexpr int ChunkHeightInPixels = 256;

private:

	struct StoreChunk
	{
		glm::ivec2 Coord;
		glm::ivec2 CenterOffset;

		uint16_t Tiles[NumTilesPerChunk];
		float AlphaValues[ChunkHeight + 1][ChunkWidth + 1][NumTilesPerChunk - 1];

		std::mutex Mutex;
	};

	struct RenderChunk
	{
		glm::ivec2 Coord;
		glm::ivec2 CenterOffset;

		uint16_t Tiles[NumTilesPerChunk];

		glm::mat4 Transform;

		li::Ref<li::VertexBuffer> AlphaVB;
		li::Ref<li::VertexArray> VertexArray;
	};

	static void LoadInitialChunks(glm::ivec2 center);
	static void LoadThreadEntryPoint(glm::ivec2 center);
	static void LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination);
	static void LoadChunkFromStore(glm::ivec2 store_coord, RenderChunk* destination);

	struct ChunkVertex
	{
		glm::vec2 Position;
		glm::vec2 TexCoord;
	};

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

	struct TerrainData
	{
		int Width;
		int Height;

		std::mutex CenterMutex;
		glm::ivec2 Center;
		glm::ivec2 PrevCenter;

		std::string TerrainPath;
		std::ifstream TerrainFile;

		std::array<StoreChunk, StoreWidth * StoreWidth> StoreChunks;
		std::atomic<bool> RunLoadThread;
		std::thread LoadThread;


		std::array<RenderChunk, RenderWidth * RenderWidth> RenderChunks;
		bool ReloadRenderChunks;
		bool RenderCenterChanged;

		li::Ref<li::Shader> TerrainShader;
		li::Ref<li::UniformBuffer> AtlasBoundsUB;
		li::Ref<li::Framebuffer> Framebuffer;

		glm::mat4 QuadTransform;
		li::Scope<li::OrthographicCamera> TerrainCamera;
	};

	static li::Scope<TerrainData> s_Data;
};