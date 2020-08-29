#include "pch.h"
#include "NoiseStore.h"

#include "SimplexNoise.h"

li::Scope<NoiseStore::NoiseData> NoiseStore::s_Data = li::CreateScope<NoiseStore::NoiseData>();

void NoiseStore::Shutdown()
{
	s_Data.reset();
}

li::Ref<li::Texture2D> NoiseStore::GetSimplex(int width, int height, int cell_size, int octaves, float persistence, float aspect_ratio)
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
	li::Ref<li::Texture2D> texture = SimplexNoise::GenerateSimplexNoiseTexture(width, height, cell_size, octaves, persistence, aspect_ratio);
	s_Data->SimplexEntries.push_back({ width, height, cell_size, octaves, persistence, aspect_ratio, texture });
	return texture;
}
