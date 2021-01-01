#include "pch.h"
#include "NoiseStore.h"

#include "SimplexNoise.h"

Li::Unique<NoiseStore::NoiseData> NoiseStore::s_Data = Li::MakeUnique<NoiseStore::NoiseData>();

void NoiseStore::Shutdown()
{
	s_Data.reset();
}

Li::Ref<Li::Texture2D> NoiseStore::GetSimplex(int width, int height, int cell_size, int octaves, float persistence, float aspect_ratio)
{
	// Check if entry exists.
	for (const auto& entry : s_Data->SimplexEntries)
	{
		if (entry.Width == width &&
			entry.Height == height &&
			entry.CellSize == cell_size &&
			entry.Octaves == octaves &&
			entry.Persistence == persistence)
		{
			return entry.Texture;
		}
	}

	// Otherwise, generate texture.
	Li::Ref<Li::Texture2D> texture = SimplexNoise::GenerateSimplexNoiseTexture(width, height, cell_size, octaves, persistence, aspect_ratio);
	s_Data->SimplexEntries.push_back({ width, height, cell_size, octaves, persistence, aspect_ratio, texture });
	return texture;
}
