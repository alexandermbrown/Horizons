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
#ifdef LI_ENABLE_ASSERTS
		LI_ASSERT(id < s_Data->Prototypes.size(), "ID out of range!");
		if (id == 0)
		{
			LI_WARN("Accessing terrain id 0. Are you sure you want this?");
		}
#endif
		return s_Data->Prototypes[id];
	}

private:

	struct TerrainData
	{
		std::vector<TerrainPrototype> Prototypes;
	};

	static li::Ref<PrototypeData> s_Lua;
	static li::Ref<TerrainData> s_Data;
};