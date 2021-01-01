#pragma once

#include "Lithium.h"

class SimplexNoise
{
public:
	static Li::Ref<Li::Texture2D> GenerateSimplexNoiseTexture(int width, int height, int cell_size, int octaves, float persistence, float aspect_ratio);

private:
	static float OctaveSimplex(float x, float y, float z, float w, int octaves, float persistence);
	static float Simplex(float x, float y, float z, float w);

	static int FastFloor(float n);
};
