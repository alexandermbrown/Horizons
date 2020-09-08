#include "pch.h"
#include "TerrainStore.h"

#include "Lithium.h"

static int positive_mod(int a, int b)
{
	return ((a % b) + b) % b;
}

TerrainStore::TerrainStore()
	: m_WorldWidth(0), m_WorldHeight(0), m_Center(0, 0), m_Open(false)
{
}

void TerrainStore::LoadTerrain(const std::string& path, glm::ivec2 center)
{
	// Load terrain file.
	m_TerrainPath = path;

	m_TerrainFile.open(path, std::ios::in | std::ios::binary);

	if (!m_TerrainFile.is_open())
	{
		LI_ERROR("Failed to open file {}", path);
		return;
	}
	m_Open = true;

	uint16_t width, height;
	m_TerrainFile.read((char*)&width, sizeof(width));
	m_TerrainFile.read((char*)&height, sizeof(height));

	m_WorldWidth = width;
	m_WorldHeight = height;

	LI_INFO("Opening terrain {} of size {},{}", path, width, height);

	LoadInitialChunks(center);

	// Start load thread.
	m_RunLoadThread = true;
	m_LoadThread = std::thread(&TerrainStore::LoadThreadEntryPoint, this, center);
}

void TerrainStore::UnloadTerrain()
{
	LI_ASSERT(m_Open, "Must be open!");
	m_RunLoadThread = false;
	m_LoadThread.join();
	m_TerrainFile.close();
	m_Open = false;
}

void TerrainStore::UpdateCenter(glm::ivec2 center)
{
	LI_ASSERT(m_Open, "Must be open!");
	std::lock_guard<std::mutex> lock(m_CenterMutex);
	m_Center = center;
}

void TerrainStore::LoadRenderChunkData(glm::ivec2 store_coord, RenderChunk* destination)
{
	LI_ASSERT(m_Open, "Must be open!");
	destination->Coord = store_coord;
	for (auto& store_chunk : m_StoreChunks)
	{
		if (store_chunk.Mutex.try_lock())
		{
			std::lock_guard<std::mutex> lock(store_chunk.Mutex, std::adopt_lock);
			if (store_chunk.Coord == store_coord)
			{
				memcpy(destination->Tiles, store_chunk.Tiles, sizeof(destination->Tiles));
				destination->AlphaVB->SetSubData((float*)store_chunk.AlphaValues, sizeof(store_chunk.AlphaValues), 0, true);
				return;
			}
		}
	}

	// Fill the alpha buffer with 0 because the chunk has not been loaded into storage yet.
	{
		LI_WARN("Chunk ({}, {}) not loaded in store.", store_coord.x, store_coord.y);

		float alpha_values[ChunkHeight + 1][ChunkWidth + 1][NumTilesPerChunk - 1] = { 0.0f };
		destination->AlphaVB->SetSubData((float*)alpha_values, sizeof(alpha_values), 0, true);
	}
}

void TerrainStore::LoadInitialChunks(glm::ivec2 center)
{
	m_Center = center;

	constexpr int HalfStoreWidth = StoreWidth / 2;
	for (int y = -HalfStoreWidth; y <= HalfStoreWidth; y++)
	{
		for (int x = -HalfStoreWidth; x <= HalfStoreWidth; x++)
		{
			glm::ivec2 coord = center + glm::ivec2{ x, y };
			// Account for any overflow past world bounds.
			coord = glm::ivec2{
				positive_mod(coord.x, m_WorldWidth),
				positive_mod(coord.y, m_WorldHeight)
			};
			auto& chunk = m_StoreChunks[(x + HalfStoreWidth) + (y + HalfStoreWidth) * (size_t)StoreWidth];
			chunk.CenterOffset = { x, y };
			LoadChunkFromDisk(coord, &chunk);
		}
	}
}

void TerrainStore::LoadThreadEntryPoint(glm::ivec2 initial_center)
{
	glm::ivec2 saved_center = initial_center;
	while (m_RunLoadThread.load())
	{
		glm::ivec2 center;
		{
			std::lock_guard<std::mutex> lock(m_CenterMutex);
			center = m_Center;
		}

		if (saved_center != center)
		{
			// test if current chunk is within radius of center.
			constexpr int radius = StoreWidth / 2;

			int left_bound = positive_mod(center.x - radius, m_WorldWidth);
			int right_bound = positive_mod(center.x + radius, m_WorldWidth);

			int bottom_bound = positive_mod(center.y - radius, m_WorldHeight);
			int top_bound = positive_mod(center.y + radius, m_WorldHeight);

			// Remove chunks which are no longer near the center.
			for (auto& chunk : m_StoreChunks)
			{
				bool in_x;
				bool in_y;

				if (left_bound < right_bound)
					in_x = chunk.Coord.x >= left_bound && chunk.Coord.x <= right_bound;
				else
					in_x = chunk.Coord.x >= left_bound || chunk.Coord.x <= right_bound;

				if (bottom_bound < top_bound)
					in_y = chunk.Coord.y >= bottom_bound && chunk.Coord.y <= top_bound;
				else
					in_y = chunk.Coord.y >= bottom_bound || chunk.Coord.y <= top_bound;

				// Swap the chunk if it is not in the radius of the center.
				if (!in_x || !in_y)
				{
					glm::ivec2 display_coord = center - chunk.CenterOffset;
					chunk.CenterOffset = -chunk.CenterOffset;

					// Account for any overflow past world bounds.
					glm::ivec2 store_coord = glm::ivec2{
						positive_mod(display_coord.x, m_WorldWidth),
						positive_mod(display_coord.y, m_WorldHeight)
					};

					LoadChunkFromDisk(store_coord, &chunk);
				}
				else
				{
					chunk.CenterOffset -= center - saved_center;
				}
			}

			saved_center = center;
		}
	}
}

void TerrainStore::LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination)
{
	std::lock_guard<std::mutex> lock(destination->Mutex);

	destination->Coord = coord;

	m_TerrainFile.seekg(sizeof(TerrainFileHeader)
		+ (size_t)coord.x * sizeof(TerrainFileChunk)
		+ (size_t)coord.y * (size_t)m_WorldWidth * sizeof(TerrainFileChunk), m_TerrainFile.beg);

	m_TerrainFile.read((char*)destination->Tiles, sizeof(destination->Tiles));

	// Load data in main (bottom left) chunk.
	TerrainFileChunk temp_chunk;
	m_TerrainFile.read((char*)temp_chunk.AlphaValues, sizeof(temp_chunk.AlphaValues));

	for (int y = 0; y < ChunkHeight; y++)
	{
		for (int x = 0; x < ChunkWidth; x++)
		{
			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				destination->AlphaValues[y][x][i] = (float)temp_chunk.AlphaValues[y][x][i] / 255.0f;
			}
		}
	}

	int right_x = (coord.x + 1) % m_WorldWidth;
	int top_y = (coord.y + 1) % m_WorldHeight;

	// Load data right.
	{
		for (int y = 0; y < ChunkHeight; y++)
		{
			m_TerrainFile.seekg(sizeof(TerrainFileHeader)
				+ (size_t)right_x * sizeof(TerrainFileChunk)
				+ (size_t)coord.y * (size_t)m_WorldWidth * sizeof(TerrainFileChunk)
				+ sizeof(TerrainFileChunk::Tiles)
				+ ((size_t)y * ChunkWidth * (NumTilesPerChunk - 1))
				, m_TerrainFile.beg);

			uint8_t bytes[NumTilesPerChunk - 1];
			m_TerrainFile.read((char*)&bytes, sizeof(bytes));

			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				destination->AlphaValues[y][ChunkWidth][i] = (float)bytes[i] / 255.0f;
			}
		}
	}

	// Load data top.
	{
		m_TerrainFile.seekg(sizeof(TerrainFileHeader)
			+ (size_t)coord.x * sizeof(TerrainFileChunk)
			+ (size_t)top_y * (size_t)m_WorldWidth * sizeof(TerrainFileChunk)
			+ sizeof(TerrainFileChunk::Tiles), m_TerrainFile.beg);

		uint8_t row[ChunkWidth][NumTilesPerChunk - 1];
		m_TerrainFile.read((char*)row, sizeof(row));

		for (int x = 0; x < ChunkWidth; x++)
		{
			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				destination->AlphaValues[ChunkHeight][x][i] = (float)row[x][i] / 255.0f;
			}
		}
	}

	// Load data top right.
	{
		m_TerrainFile.seekg(sizeof(TerrainFileHeader)
			+ (size_t)right_x * sizeof(TerrainFileChunk)
			+ (size_t)top_y * (size_t)m_WorldWidth * sizeof(TerrainFileChunk)
			+ sizeof(TerrainFileChunk::Tiles), m_TerrainFile.beg);

		uint8_t bytes[3];
		m_TerrainFile.read((char*)&bytes, sizeof(bytes));

		for (int i = 0; i < NumTilesPerChunk - 1; i++)
		{
			destination->AlphaValues[ChunkHeight][ChunkWidth][i] = (float)bytes[i] / 255.0f;
		}
	}
}
