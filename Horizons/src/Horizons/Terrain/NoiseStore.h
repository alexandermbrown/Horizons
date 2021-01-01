#pragma once

#include <vector>
#include "Lithium.h"

class NoiseStore
{
public:
	static void Shutdown();
	static Li::Ref<Li::Texture2D> GetSimplex(int width, int height, int cell_size, int octaves, float persistence, float aspect_ratio);

private:

	struct SimplexEntry
	{
		int Width;
		int Height;
		int CellSize;
		int Octaves;
		float Persistence;
		float AspectRatio;

		Li::Ref<Li::Texture2D> Texture;
	};
	
	struct NoiseData
	{
		std::vector<SimplexEntry> SimplexEntries;
	};

	static Li::Unique<NoiseData> s_Data;
};