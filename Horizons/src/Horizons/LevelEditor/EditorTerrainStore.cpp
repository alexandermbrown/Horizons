#include "pch.h"
#include "EditorTerrainStore.h"

#include "Horizons/Core/Math.h"
#include "Horizons/Terrain/TerrainRenderer.h"

EditorTerrainStore::EditorTerrainStore()
	: m_Open(false), m_WorldWidth(0), m_WorldHeight(0), m_StoreChunks(nullptr)
{
}

EditorTerrainStore::~EditorTerrainStore()
{
}

bool EditorTerrainStore::LoadTerrain(const std::string& path, glm::ivec2 center)
{
	// Load terrain file.
	m_TerrainFile.open(path, std::ios::in | std::ios::binary);
	if (!m_TerrainFile.is_open())
	{
		LI_ERROR("Failed to open terrain file {}", path);
		return false;
	}
	m_TerrainPath = path;
	m_Open = true;

	uint16_t width, height;
	m_TerrainFile.read((char*)&width, sizeof(width));
	m_TerrainFile.read((char*)&height, sizeof(height));

	m_WorldWidth = width;
	m_WorldHeight = height;

	m_StoreChunks = new StoreChunk[m_WorldWidth * m_WorldHeight];

	LI_INFO("Loading {}x{} terrain in editor: {}", width, height, path);

	for (int y = 0; y < m_WorldHeight; y++)
	{
		for (int x = 0; x < m_WorldWidth; x++)
		{
			LoadChunkFromDisk({ x, y }, &m_StoreChunks[x + y * m_WorldWidth]);
		}
	}

	return true;
}

void EditorTerrainStore::UnloadTerrain()
{
	LI_ASSERT(m_Open, "Must be open!");
	m_TerrainFile.close();
	delete m_StoreChunks;
	m_StoreChunks = nullptr;
	m_Open = false;
}

void EditorTerrainStore::LoadRenderChunkData(glm::ivec2 store_coord, RenderChunk* destination)
{
	destination->Coord = store_coord;
	if (store_coord.x >= 0 && store_coord.y >= 0 && store_coord.x < m_WorldWidth && store_coord.y < m_WorldHeight)
	{
		StoreChunk& store_chunk = m_StoreChunks[store_coord.x + store_coord.y * m_WorldWidth];
		memcpy(destination->Tiles, store_chunk.Tiles, sizeof(destination->Tiles));
		destination->AlphaVB->SetSubData((float*)store_chunk.AlphaValues, sizeof(store_chunk.AlphaValues), 0, true);
	}
	else
	{
		LI_ERROR("Chunk coordinate {},{} out of bounds.", store_coord.x, store_coord.y);
		AlphaValuesArray alpha_values = { 0.0f };
		destination->AlphaVB->SetSubData((float*)alpha_values, sizeof(alpha_values), 0, true);
	}
}

void EditorTerrainStore::ApplyBrush(Brush* brush, glm::vec2 brush_pos, float dt)
{
	const float left_bound = Math::PositiveMod(brush_pos.x - brush->OuterRadius, (float)m_WorldWidth);
	const float right_bound = Math::PositiveMod(brush_pos.x + brush->OuterRadius, (float)m_WorldWidth);

	const float bottom_bound = Math::PositiveMod(brush_pos.y - brush->OuterRadius, (float)m_WorldHeight);
	const float top_bound = Math::PositiveMod(brush_pos.y + brush->OuterRadius, (float)m_WorldHeight);

	for (int i = 0; i < m_WorldWidth * m_WorldHeight; i++)
	{
		StoreChunk& chunk = m_StoreChunks[i];
		bool in_x;
		bool in_y;

		const float chunk_x = chunk.Coord.x * TerrainRenderer::MetersPerChunk;
		const float chunk_y = chunk.Coord.y * TerrainRenderer::MetersPerChunk;
		if (left_bound < right_bound)
			in_x = chunk_x >= left_bound && chunk_x <= right_bound;
		else
			in_x = chunk_x >= left_bound || chunk_x <= right_bound;

		if (bottom_bound < top_bound)
			in_y = chunk_y >= bottom_bound && chunk_y <= top_bound;
		else
			in_y = chunk_y >= bottom_bound || chunk_y <= top_bound;

		if (in_x && in_y)
		{
			ApplyBrushToChunk(brush, brush_pos, dt, chunk);
		}
	}
}

void EditorTerrainStore::LoadChunkFromDisk(glm::ivec2 coord, StoreChunk* destination)
{
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

void EditorTerrainStore::ApplyBrushToChunk(Brush* brush, glm::vec2 brush_pos, float dt, StoreChunk& chunk)
{
	for (int y = 0; y < ChunkHeight; y++)
	{
		for (int x = 0; x < ChunkWidth; x++)
		{

		}
	}
}
