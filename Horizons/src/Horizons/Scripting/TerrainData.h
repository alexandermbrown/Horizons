#pragma once

#include "Lithium.h"
#include "entt/entt.hpp"

#include <stdint.h>
#include <vector>

struct TerrainType
{
	uint16_t ID;
	std::string Name;
	std::string Atlas;
	float NoiseWeight;
	float BlendWidth;
	Li::Ref<Li::Texture2D> NoiseTexture;
};

class TerrainData
{
public:
	void LoadTypes();

	const TerrainType& GetTerrainPrototype(uint16_t id) const;

private:
	std::vector<TerrainType> m_Types;
};
