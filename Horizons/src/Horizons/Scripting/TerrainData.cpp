#include "pch.h"
#include "TerrainData.h"

#include "Horizons.h"
#include "Horizons/Terrain/NoiseStore.h"
#include "TerrainData.h"

void TerrainData::LoadTypes()
{
	sol::state& lua = Li::Application::Get<Horizons>().GetScriptContext().GetLua();

	sol::table app = lua["App"];
	if (!app.valid())
	{
		Li::Log::Error("Error reading global 'App'");
		return;
	}

	sol::table terrain_list = app["terrain"];
	if (!terrain_list.valid())
	{
		Li::Log::Error("Error reading 'prototypes.terrain'");
		return;
	}

	m_Types.clear();
	m_Types.reserve(terrain_list.size());
	m_Types.push_back({ 0, "test_small", "atlas_test", 0.1f, 0.0f, nullptr });

	for (int index = 1; index <= terrain_list.size(); index++)
	{
		sol::optional<sol::table> optional_terrain = terrain_list[index];
		if (optional_terrain)
		{
			sol::table terrain = optional_terrain.value();
			TerrainType terrain_struct;

			// ID.
			sol::optional<uint16_t> id = terrain["id"];
			if (id)
			{
				if (id.value() == index)
					terrain_struct.ID = id.value();
				else
				{
					Li::Log::Error("Unaliagned terrain id: {} at index {}. Ensure id and index are equal.", id.value(), index);
					break;
				}
			}
			else
			{
				Li::Log::Error("Error reading integer 'id' in terrain at index {}.", index);
				break;
			}
			// Name.
			sol::optional<std::string> name = terrain["name"];
			if (name)
				terrain_struct.Name = name.value();
			else
			{
				Li::Log::Error("Error reading string 'name' in terrain with id.", index);
				break;
			}
			// Atlas Name.
			sol::optional<std::string> atlas = terrain["atlas"];
			if (atlas)
				terrain_struct.Atlas = atlas.value();
			else
			{
				Li::Log::Error("Error reading string 'atlas' in terrain with id {}.", index);
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
				Li::Log::Error("Blend width ({}) in terrain with id {} must be between 0.0 and 1.0. ", terrain_struct.BlendWidth, index);
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
						Li::Log::Error("Unknown noise type '{}' in terrain with id {}", type.value(), index);
						break;
					}
				}
				else
				{
					Li::Log::Error("Missing string 'type' in 'noise' in terrain with id {}", index);
				}
			}
			else
			{
				terrain_struct.NoiseTexture = nullptr;
			}

			m_Types.push_back(std::move(terrain_struct));
		}
		else
		{
			Li::Log::Error("Invalid element in terrain at index {}.", index);
			break;
		}
	}
}

const TerrainType& TerrainData::GetTerrainPrototype(uint16_t id) const
{
	LI_ASSERT(m_Types.size() > 0, "Terrain types not loaded!");

	if (id < m_Types.size())
		return m_Types.at(id);
	else
	{
		Li::Log::Error("ID out of range!");
		return m_Types.at(0);
	}
}
