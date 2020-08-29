

local terrain = {
	{
		id = 1,
		name = "terrain_dirt",
		atlas = "atlas_terrain_earth",
		noise_weight = 0.1,
		blend_width = 0.1,
		noise = {
			type = "simplex",
			cell_size = 16,
			octaves = 4,
			persistence = 0.6
		}
	},
	{
		id = 2,
		name = "terrain_grass",
		atlas = "atlas_terrain_earth",
		noise_weight = 0.4,
		blend_width = 0.1,
		noise = {
			type = "simplex",
			cell_size = 16,
			octaves = 4,
			persistence = 0.4
		}
	},
	{
		id = 3,
		name = "terrain_stone",
		atlas = "atlas_terrain_earth",
		noise_weight = 0.3,
		blend_width = 0.0,
		noise = {
			type = "simplex",
			cell_size = 16,
			octaves = 3,
			persistence = 0.4
		}
	},
	{
		id = 4,
		name = "terrain_sand",
		atlas = "atlas_terrain_earth",
		noise_weight = 0.4,
		blend_width = 0.3,
		noise = {
			type = "simplex",
			cell_size = 8,
			octaves = 4,
			persistence = 0.8
		}
	}
}

return terrain