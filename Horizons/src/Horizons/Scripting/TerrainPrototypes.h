#pragma once

#include "PrototypeData.h"

#include "Lithium.h"
#include "entt/entt.hpp"

#include <stdint.h>
#include <vector>

struct TerrainPrototype
{
	uint16_t ID;
	std::string Name;
	std::string Atlas;
	float NoiseWeight;
	float BlendWidth;
	li::Ref<li::Texture2D> NoiseTexture;
};

class TerrainPrototypes
{
public:
	static void Init(li::Ref<PrototypeData> data);
	static void Shutdown();

	static void InstantiatePrototypes();

	static inline const TerrainPrototype& GetTerrainPrototype(uint16_t id)
	{
		if (id < s_Data->Prototypes.size())
		{
			return s_Data->Prototypes.at(id);
		}
		else
		{
			LI_ERROR("ID out of range!");
			return s_Data->Prototypes.at(0);
		}
	}

private:
	struct TerrainData
	{
		std::vector<TerrainPrototype> Prototypes;
	};

	static li::Ref<PrototypeData> s_Lua;
	static li::Ref<TerrainData> s_Data;
};