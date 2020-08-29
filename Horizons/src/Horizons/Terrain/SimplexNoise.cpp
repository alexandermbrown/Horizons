#include "pch.h"
#include "SimplexNoise.h"

#include "glm/glm.hpp"

// Sources:
// https://adrianb.io/2014/08/09/perlinnoise.html
// http://weber.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java
// https://ronvalstar.nl/creating-tileable-noise-maps

// permutations: arranged array of all numbers from 0-255 inclusive.
static const int perm[] = { 151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

	151,160,137,91,90,15,
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

// 4D gradients.
static const glm::vec4 grad4[] = {
	{0,1,1,1}, {0,1,1,-1},{0,1,-1,1},{0,1,-1,-1},
	{0,-1,1,1},{0,-1,1,-1},{0,-1,-1,1},{0,-1,-1,-1},
	{1,0,1,1},{1,0,1,-1},{1,0,-1,1},{1,0,-1,-1},
	{-1,0,1,1},{-1,0,1,-1},{-1,0,-1,1},{-1,0,-1,-1},
	{1,1,0,1},{1,1,0,-1},{1,-1,0,1},{1,-1,0,-1},
	{-1,1,0,1},{-1,1,0,-1},{-1,-1,0,1},{-1,-1,0,-1},
	{1,1,1,0},{1,1,-1,0},{1,-1,1,0},{1,-1,-1,0},
	{-1,1,1,0},{-1,1,-1,0},{-1,-1,1,0},{-1,-1,-1,0}
};

static const float F4 = (std::sqrt(5.0f) - 1.0f) / 4.0f;
static const float G4 = (5.0f - std::sqrt(5.0f)) / 20.0f;

li::Ref<li::Texture2D> SimplexNoise::GenerateSimplexNoiseTexture(int width, int height, int cell_size, int octaves, float persistence, float aspect_ratio)
{
	uint8_t* data = new uint8_t[(size_t)width * (size_t)height];

	LI_ASSERT(width / cell_size <= 256 && height / cell_size <= 256, "Width and height must be less than 256.");

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Create a tileable texture by sampling from two circles in four dimensional noise space.
			float x_frac = (float)x / (float)width;
			float y_frac = (float)y / (float)height;
			float x_angle = x_frac * 2.0f * (float)M_PI;
			float y_angle = y_frac * 2.0f * (float)M_PI;
			float sx = (std::sin(x_angle) + 1.0f) * 128.0f / (float)cell_size / (float)M_PI * (float)width / (float)height;
			float sy = (std::cos(x_angle) + 1.0f) * 128.0f / (float)cell_size / (float)M_PI * (float)width / (float)height;
			float sz = (std::sin(y_angle) + 1.0f) * 128.0f / (float)cell_size / (float)M_PI * aspect_ratio;
			float sw = (std::cos(y_angle) + 1.0f) * 128.0f / (float)cell_size / (float)M_PI * aspect_ratio;

			float value = OctaveSimplex(sx, sy, sz, sw, octaves, persistence);
			data[x + y * width] = (uint8_t)((value + 1.0f) * 128.0f);
		}
	}

	li::Ref<li::Texture2D> texture = li::Texture2D::Create(width, height, data, li::WrapType::Repeat, li::WrapType::Repeat, li::FilterType::Linear, li::FilterType::Nearest, 1);
	delete[] data;
	return texture;
}

float SimplexNoise::OctaveSimplex(float x, float y, float z, float w, int octaves, float persistence)
{
	float total = 0.0f;
	float frequency = 1.0f;
	float amplitude = 1.0f;
	float max = 0.0f;

	for (int i = 0; i < octaves; i++)
	{
		total += Simplex(x * frequency, y * frequency, z * frequency, w * frequency) * amplitude;
		max += amplitude;
		amplitude *= persistence;
		frequency *= 2;
	}

	return total / max;
}

float SimplexNoise::Simplex(float x, float y, float z, float w)
{
	float n0, n1, n2, n3, n4;

	float s = (x + y + z + w) * F4;
	int i = FastFloor(x + s);
	int j = FastFloor(y + s);
	int k = FastFloor(z + s);
	int l = FastFloor(w + s);
	float t = (i + j + k + l) * G4;
	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;
	float W0 = l - t;
	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;
	float w0 = w - W0;

	int rankx = 0;
	int ranky = 0;
	int rankz = 0;
	int rankw = 0;
	if (x0 > y0) rankx++; else ranky++;
	if (x0 > z0) rankx++; else rankz++;
	if (x0 > w0) rankx++; else rankw++;
	if (y0 > z0) ranky++; else rankz++;
	if (y0 > w0) ranky++; else rankw++;
	if (z0 > w0) rankz++; else rankw++;
	int i1, j1, k1, l1;
	int i2, j2, k2, l2;
	int i3, j3, k3, l3;
	// Rank 3 denotes the largest coordinate.
	i1 = rankx >= 3 ? 1 : 0;
	j1 = ranky >= 3 ? 1 : 0;
	k1 = rankz >= 3 ? 1 : 0;
	l1 = rankw >= 3 ? 1 : 0;
	// Rank 2 denotes the second largest coordinate.
	i2 = rankx >= 2 ? 1 : 0;
	j2 = ranky >= 2 ? 1 : 0;
	k2 = rankz >= 2 ? 1 : 0;
	l2 = rankw >= 2 ? 1 : 0;
	// Rank 1 denotes the second smallest coordinate.
	i3 = rankx >= 1 ? 1 : 0;
	j3 = ranky >= 1 ? 1 : 0;
	k3 = rankz >= 1 ? 1 : 0;
	l3 = rankw >= 1 ? 1 : 0;

	float x1 = x0 - i1 + G4;
	float y1 = y0 - j1 + G4;
	float z1 = z0 - k1 + G4;
	float w1 = w0 - l1 + G4;
	float x2 = x0 - i2 + 2.0f * G4;
	float y2 = y0 - j2 + 2.0f * G4;
	float z2 = z0 - k2 + 2.0f * G4;
	float w2 = w0 - l2 + 2.0f * G4;
	float x3 = x0 - i3 + 3.0f * G4;
	float y3 = y0 - j3 + 3.0f * G4;
	float z3 = z0 - k3 + 3.0f * G4;
	float w3 = w0 - l3 + 3.0f * G4;
	float x4 = x0 - 1.0f + 4.0f * G4;
	float y4 = y0 - 1.0f + 4.0f * G4;
	float z4 = z0 - 1.0f + 4.0f * G4;
	float w4 = w0 - 1.0f + 4.0f * G4;

	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int ll = l & 255;
	int gi0 = perm[ii + perm[jj + perm[kk + perm[ll]]]] % 32;
	int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]] % 32;
	int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]] % 32;
	int gi3 = perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]] % 32;
	int gi4 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]] % 32;

	float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
	if (t0 < 0.0f) n0 = 0.0f;
	else {
		t0 *= t0;
		n0 = t0 * t0 * glm::dot(grad4[gi0], { x0, y0, z0, w0 });
	}
	float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
	if (t1 < 0) n1 = 0.0f;
	else {
		t1 *= t1;
		n1 = t1 * t1 * glm::dot(grad4[gi1], { x1, y1, z1, w1 });
	}
	float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
	if (t2 < 0.0f) n2 = 0.0f;
	else {
		t2 *= t2;
		n2 = t2 * t2 * glm::dot(grad4[gi2], { x2, y2, z2, w2 });
	}
	float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
	if (t3 < 0.0f) n3 = 0.0f;
	else {
		t3 *= t3;
		n3 = t3 * t3 * glm::dot(grad4[gi3], { x3, y3, z3, w3 });
	}
	float t4 = 0.6f - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
	if (t4 < 0.0f) n4 = 0.0f;
	else {
		t4 *= t4;
		n4 = t4 * t4 * glm::dot(grad4[gi4], { x4, y4, z4, w4 });
	}
	// Sum up and scale the result to cover the range [-1,1]
	return 27.0f * (n0 + n1 + n2 + n3 + n4);
}

int SimplexNoise::FastFloor(float n)
{
	int ni = (int)n;
	return n < ni ? ni - 1 : ni;
}
