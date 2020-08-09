#pragma once

#include <stdint.h>

#define HZ_TERRAIN_TEXTURE_SLOT_COUNT 4
#define HZ_CHUNK_WIDTH 16
#define HZ_CHUNK_HEIGHT 8
#define HZ_MAX_VISIBLE_CHUNKS_WIDTH 3
#define HZ_CHUNK_BLOCK_WIDTH 4

struct TerrainChunk
{
	int32_t X, Y;

	// Stores up to 4 tile id's used.
	// index 0 is the bottom, always solid color.
	uint16_t Tiles[HZ_TERRAIN_TEXTURE_SLOT_COUNT];

	float Data[HZ_CHUNK_HEIGHT][HZ_CHUNK_WIDTH][HZ_TERRAIN_TEXTURE_SLOT_COUNT - 1];
};


struct TerrainWorld
{
	uint32_t ID;

	// Width and height of world in Chunks.
	// Must be a multiple of HZ_CHUNK_BLOCK_WIDTH (4).
	uint32_t Width, Height;

	TerrainChunk ChunkCache[HZ_MAX_VISIBLE_CHUNKS_WIDTH * 3][HZ_MAX_VISIBLE_CHUNKS_WIDTH * 3];
};


/*
struct (disk)TerrainChunk
{
	// Stores up to 4 tile id's used.
	// index 0 is the bottom, always solid color.
	uint16_t Tiles[HZ_TERRAIN_TEXTURE_SLOT_COUNT];

	// NOTE: num used is variable, depending on how many
	uint8_t Data[ num used ][HZ_CHUNK_HEIGHT][HZ_CHUNK_WIDTH];
};

// World as on disk.
struct (disk)TerrainWorld
{
	char FileSignature[4] = { 'W', 'R', 'L', 'D' };
	uint32_t ID;
	uint32_t Width, Height;

	// Not actually fixed, variable size on disk, nearby chunks cached in memory.
	Chunk chunks[Width][Height];
};

Vertex Buffer Layout:

vec2 a_Position
vec4 a_LayerAlpha


Uniforms:

uniform sampler2D u_Texture;

*/