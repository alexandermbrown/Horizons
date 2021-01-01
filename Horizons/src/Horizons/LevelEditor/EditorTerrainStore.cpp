#include "pch.h"
#ifndef LI_DIST
#include "EditorTerrainStore.h"

#include "Horizons/Core/Math.h"
#include "Horizons/Terrain/TerrainRenderer.h"

#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

EditorTerrainStore::EditorTerrainStore()
	: m_Open(false), m_Modified(false), m_ReloadRenderChunks(false), m_WorldWidth(0), m_WorldHeight(0), m_StoreChunks(nullptr)
{
}

EditorTerrainStore::~EditorTerrainStore()
{
}

bool EditorTerrainStore::LoadTerrain(const std::string& path, glm::ivec2 center)
{
	// Load terrain file.
	m_TerrainFile.open(path, std::ios::in | std::ios::out | std::ios::binary);
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

void EditorTerrainStore::ApplyBrush(BrushSettings* brush, glm::vec2 brush_pos, int layer, Li::Duration::us dt)
{
	const float world_width = (float)m_WorldWidth * TerrainRenderer::MetersPerChunk;
	const float world_height = (float)m_WorldHeight * TerrainRenderer::MetersPerChunk;

	const float brush_left = Math::PositiveMod(brush_pos.x - brush->OuterRadius, world_width);
	const float brush_right = Math::PositiveMod(brush_pos.x + brush->OuterRadius, world_width);
	const float brush_bottom = Math::PositiveMod(brush_pos.y - brush->OuterRadius, world_height);
	const float brush_top = Math::PositiveMod(brush_pos.y + brush->OuterRadius, world_height);

	brush_pos = {
		Math::PositiveMod(brush_pos.x, world_width),
		Math::PositiveMod(brush_pos.y, world_height)
	};

	for (int i = 0; i < m_WorldWidth * m_WorldHeight; i++)
	{
		StoreChunk& chunk = m_StoreChunks[i];
		bool in_x;
		bool in_y;
		
		const float chunk_left = chunk.Coord.x * TerrainRenderer::MetersPerChunk;
		const float chunk_bottom = chunk.Coord.y * TerrainRenderer::MetersPerChunk;
		const float chunk_right = chunk_left + TerrainRenderer::MetersPerChunk;
		const float chunk_top = chunk_bottom + TerrainRenderer::MetersPerChunk;

		if (brush_left <= brush_right)
			in_x = brush_right >= chunk_left && brush_left <= chunk_right;
		else
			in_x = brush_right >= chunk_left || brush_left <= chunk_right;

		if (brush_bottom <= brush_top)
			in_y = brush_top >= chunk_bottom && brush_bottom <= chunk_top;
		else
			in_y = brush_top >= chunk_bottom || brush_bottom <= chunk_top;
		
		if (in_x && in_y)
		{
			ApplyBrushToChunk(brush, brush_pos, layer, dt, chunk);
		}
	}
}

void EditorTerrainStore::Save()
{
	for (int y = 0; y < m_WorldHeight; y++)
	{
		for (int x = 0; x < m_WorldWidth; x++)
		{
			StoreChunk& chunk = m_StoreChunks[x + y * m_WorldWidth];
			if (chunk.Modified)
			{
				m_TerrainFile.seekg(sizeof(TerrainFileHeader)
					+ (size_t)chunk.Coord.x * sizeof(TerrainFileChunk)
					+ (size_t)chunk.Coord.y * (size_t)m_WorldWidth * sizeof(TerrainFileChunk), std::ios::beg);
				SaveChunk(chunk);
			}
		}
	}
	m_Modified = false;
}

bool EditorTerrainStore::SaveAs(const std::string& path)
{
	{
		std::fstream new_file(path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		if (!new_file.is_open())
		{
			LI_ERROR("Failed to open terrain file {}", path);
			return false;
		}
		m_TerrainFile.clear();
		m_TerrainFile.close();
		m_TerrainFile = std::move(new_file);
	}
	LI_ASSERT(m_TerrainFile.is_open() && m_TerrainFile.good(), "Terrain file should be open.");
	m_TerrainPath = path;
	OverwriteWorld();
	m_Modified = false;
	return true;
}

bool EditorTerrainStore::ReloadRenderChunks()
{
	bool value = m_ReloadRenderChunks;
	m_ReloadRenderChunks = false;
	return value;
}

bool EditorTerrainStore::CreateEmptyTerrainFile(const std::string& path, glm::ivec2 world_size)
{
	// Load terrain file.
	std::ofstream new_file(path, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!new_file.is_open())
	{
		LI_ERROR("Failed to open terrain file {}", path);
		return false;
	}

	uint16_t width = world_size.x;
	uint16_t height = world_size.y;
	new_file.write((char*)&width, sizeof(width));
	new_file.write((char*)&height, sizeof(height));

	for (int y = 0; y < world_size.y; y++)
	{
		for (int x = 0; x < world_size.x; x++)
		{
			for (int i = 0; i < NumTilesPerChunk; i++)
			{
				uint16_t tile = i + 1;
				new_file.write((const char*)&tile, sizeof(tile));
			}
			for (int y = 0; y < ChunkHeight; y++)
			{
				for (int x = 0; x < ChunkWidth; x++)
				{
					// Layers 1-3 are fully transparent.
					uint8_t value = 0;
					new_file.write((const char*)&value, sizeof(value));
					new_file.write((const char*)&value, sizeof(value));
					new_file.write((const char*)&value, sizeof(value));
				}
			}
		}
	}
	return true;
}

void EditorTerrainStore::ApplyBrushToChunk(BrushSettings* brush, glm::vec2 brush_pos, int layer, Li::Duration::us dt, StoreChunk& chunk)
{
	const float world_width = (float)m_WorldWidth * TerrainRenderer::MetersPerChunk;
	const float world_height = (float)m_WorldHeight * TerrainRenderer::MetersPerChunk;

	bool reverse = Li::Application::Get().GetInput().IsKeyPressed(SDL_SCANCODE_LSHIFT);

	for (int y = 0; y <= ChunkHeight; y++)
	{
		for (int x = 0; x <= ChunkWidth; x++)
		{
			constexpr float vertex_distance_x = TerrainRenderer::MetersPerChunk / (float)ChunkWidth;
			constexpr float vertex_distance_y = TerrainRenderer::MetersPerChunk / (float)ChunkHeight;
			glm::vec2 vert = {
				chunk.Coord.x * TerrainRenderer::MetersPerChunk + x * vertex_distance_x,
				chunk.Coord.y * TerrainRenderer::MetersPerChunk + y * vertex_distance_y
			};

			// Calculate shortest distance from the vertex to the brush.
			float dx1 = vert.x - brush_pos.x;
			float dx2;
			if (brush_pos.x >= vert.x)
				dx2 = dx1 + world_width;
			else
				dx2 = dx1 - world_width;

			float dy1 = vert.y - brush_pos.y;
			float dy2;
			if (brush_pos.y >= vert.y)
				dy2 = dy1 + world_height;
			else
				dy2 = dy1 - world_height;

			float distance = glm::length(glm::vec2{
				std::abs(dx1) <= std::abs(dx2) ? dx1 : dx2,
				std::abs(dy1) <= std::abs(dy2) ? dy1 : dy2
			});

			float brush_power = 1.0f - std::clamp((distance - brush->InnerRadius) / (brush->OuterRadius - brush->InnerRadius), 0.0f, 1.0f);

			float prev_value = chunk.AlphaValues[y][x][layer];
			float new_value;
			if (brush->Subtract != reverse)
			{
				new_value = std::max(prev_value - brush_power * brush->Strength * Li::Duration::fsec(dt).count(), 0.0f);
			}
			else
			{
				new_value = std::min(prev_value + brush_power * brush->Strength * Li::Duration::fsec(dt).count(), 1.0f);
			}

			if (new_value != prev_value)
			{
				chunk.AlphaValues[y][x][layer] = new_value;
				chunk.Modified = true;
				m_ReloadRenderChunks = true;
				m_Modified = true;
			}
		}
	}
}

void EditorTerrainStore::OverwriteWorld()
{
	// We should already be at the start of the file.
	uint16_t world_width = (uint16_t)m_WorldWidth;
	uint16_t world_height = (uint16_t)m_WorldHeight;
	m_TerrainFile.write((const char*)&world_width, sizeof(world_width));
	m_TerrainFile.write((const char*)&world_height, sizeof(world_height));

	for (int y = 0; y < m_WorldHeight; y++)
	{
		for (int x = 0; x < m_WorldWidth; x++)
		{
			SaveChunk(m_StoreChunks[x + y * m_WorldWidth]);
		}
	}
}

void EditorTerrainStore::SaveChunk(StoreChunk& chunk)
{
	// Assumes we are already in the correct file position.

	for (int i = 0; i < NumTilesPerChunk; i++)
	{
		m_TerrainFile.write((const char*)&chunk.Tiles[i], sizeof(chunk.Tiles[i]));
	}

	for (int y = 0; y < ChunkHeight; y++)
	{
		for (int x = 0; x < ChunkWidth; x++)
		{
			auto& alphas = chunk.AlphaValues[y][x];
			for (int i = 0; i < NumTilesPerChunk - 1; i++)
			{
				float alpha = alphas[i];
				uint8_t byte = (uint8_t)std::floor(alpha >= 1.0f ? 255 : alpha * 256.0f);

				m_TerrainFile.write((const char*)&byte, sizeof(byte));
			}
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
				destination->AlphaValues[y][x][i] = (float)temp_chunk.AlphaValues[y][x][i] / 256.0f;
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
				destination->AlphaValues[y][ChunkWidth][i] = (float)bytes[i] / 256.0f;
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
				destination->AlphaValues[ChunkHeight][x][i] = (float)row[x][i] / 256.0f;
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
			destination->AlphaValues[ChunkHeight][ChunkWidth][i] = (float)bytes[i] / 256.0f;
		}
	}
}
#endif
