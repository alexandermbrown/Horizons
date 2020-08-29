#include "pch.h"
#include "TerrainPrototypes.h"

#include "Horizons/Terrain/NoiseStore.h"

li::Ref<PrototypeData> TerrainPrototypes::s_Lua = nullptr;
li::Ref<TerrainPrototypes::TerrainData> TerrainPrototypes::s_Data = nullptr;

void TerrainPrototypes::Init(li::Ref<PrototypeData> data)
{
	s_Lua = data;
	s_Data = li::CreateScope<TerrainData>();
}

void TerrainPrototypes::Shutdown()
{
	s_Lua.reset();
	s_Data.reset();
}

void TerrainPrototypes::InstantiatePrototypes()
{
	auto prototypes = s_Lua->Lua["prototypes"];
	if (!prototypes.valid())
	{
		LI_ERROR("Error reading global 'prototypes'");
		return;
	}

	sol::table terrain_list = prototypes["terrain"];
	if (!terrain_list.valid())
	{
		LI_ERROR("Error reading 'prototypes.terrain'");
		return;
	}

	s_Data->Prototypes.clear();
	s_Data->Prototypes.reserve(terrain_list.size());
	s_Data->Prototypes.push_back({ 0, "test_small", "texture_default", 0.1f, 0.0f, nullptr });

	for (int index = 1; index <= terrain_list.size(); index++)
	{
		sol::optional<sol::table> optional_terrain = terrain_list[index];
		if (optional_terrain)
		{
			sol::table terrain = optional_terrain.value();
			TerrainPrototype terrain_struct;

			// ID.
			sol::optional<uint16_t> id = terrain["id"];
			if (id)
			{
				if (id.value() == index)
				{
					terrain_struct.ID = id.value();
				}
				else
				{
					LI_ERROR("Unaliagned terrain id: {} at index {}. Ensure id and index are equal.", id.value(), index);
					break;
				}
			}
			else
			{
				LI_ERROR("Error reading integer 'id' in terrain at index {}.", index);
				break;
			}
			// Name.
			sol::optional<std::string> name = terrain["name"];
			if (name)
			{
				terrain_struct.Name = name.value();
			}
			else
			{
				LI_ERROR("Error reading string 'name' in terrain with id.", index);
				break;
			}
			// Atlas Name.
			sol::optional<std::string> atlas = terrain["atlas"];
			if (atlas)
			{
				terrain_struct.Atlas = atlas.value();
			}
			else
			{
				LI_ERROR("Error reading string 'atlas' in terrain with id {}.", index);
				break;
			}
			// Noise amplitude.
			sol::optional<float> noise_weight = terrain["noise_weight"];
			terrain_struct.NoiseWeight = noise_weight.value_or(0.1f);
			// Blend width.
			sol::optional<float> blend_width = terrain["blend_width"];
			terrain_struct.BlendWidth = blend_width.value_or(0.0f);
			if (terrain_struct.BlendWidth < 0.0f || terrain_struct.BlendWidth > 1.0f)
			{
				terrain_struct.BlendWidth = 0.0f;
				LI_ERROR("Blend width ({}) in terrain with id {} must be between 0.0 and 1.0. ", terrain_struct.BlendWidth, index);
			}
			// Noise.
			sol::optional<sol::table> noise_optional = terrain["noise"];
			if (noise_optional)
			{
				sol::table noise_table = noise_optional.value();
				sol::optional<const char*> type = noise_table["type"];
				if (type)
				{
					if (!strcmp(type.value(), "simplex"))
					{
						sol::optional<int> cell_size = noise_table["cell_size"];
						sol::optional<int> octaves = noise_table["octaves"];
						sol::optional<float> persistence = noise_table["persistence"];

						terrain_struct.NoiseTexture = NoiseStore::GetSimplex(512, 256, cell_size.value_or(16), octaves.value_or(4), persistence.value_or(0.4f), 2.0f);
					}
					else
					{
						LI_ERROR("Unknown noise type '{}' in terrain with id {}", type.value(), index);
						break;
					}
				}
				else
				{
					LI_ERROR("Missing string 'type' in 'noise' in terrain with id {}", index);
				}
			}
			else
			{
				terrain_struct.NoiseTexture = nullptr;
			}

			s_Data->Prototypes.push_back(std::move(terrain_struct));
		}
		else
		{
			LI_ERROR("Invalid element in terrain at index {}.", index);
			break;
		}
	}
}
