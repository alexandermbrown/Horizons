local bit = require("bit")
require "data.scripts.prototypes.ui_enums"

local CreateMainMenuButton = function(name, text)
	return {
		name = name,
		layout_contain = LAYOUT_CONTAIN.COLUMN,
		layout_behave = LAYOUT_BEHAVE.TOP,
		width = 400, height = 70,
		margins = { top = 16, bottom = 16 },
		color = { 0.3, 0.5, 0.4, 0.2 },
		OnMouseEnter = function(registry, entity)
			SetColor(registry, entity, { 0.3, 0.5, 0.4, 0.3 })
		end,
		OnMouseLeave = function(registry, entity)
			SetColor(registry, entity, { 0.3, 0.5, 0.4, 0.2 })
		end,
		children = {
			{
				layout_contain = LAYOUT_CONTAIN.COLUMN,
				layout_behave = LAYOUT_BEHAVE.LEFT,
				width = 1, height = 1,
				color = { 1.0, 1.0, 1.0, 0.9 },
				margins = { top = 60, left = 60 },
				label = {
					text = text,
					pt_size = 40,
					font = "Tomorrow-Regular"
				}
			}
		}
	}
end

local bg_nebula = {  -- Background nebula
	layout_contain = LAYOUT_CONTAIN.COLUMN,
	layout_behave = LAYOUT_BEHAVE.FILL,
	texture = "space_scene_1_nebula",
	texture_crop = true,
	children = {
		CreateMainMenuButton("button_play", "Play"),
		CreateMainMenuButton("button_level_editor", "Level Editor"),
		CreateMainMenuButton("button_example_video", "Video")
	}
}

local bg_stars = {	-- Background stars.
	layout_contain = LAYOUT_CONTAIN.COLUMN,
	layout_behave = LAYOUT_BEHAVE.FILL,
	texture = "space_scene_1_stars",
	texture_crop = true,
	children = { bg_nebula }
}

local bg_star_glow = {	-- Background star glow
	layout_contain = LAYOUT_CONTAIN.COLUMN,
	layout_behave = LAYOUT_BEHAVE.FILL,
	texture = "space_scene_1_star_glow",
	texture_crop = true,
	flicker = {
		magnitude = 0.2,
		delay = 0.1
	},
	children = { bg_stars }
}

local root = {
	layout_contain = LAYOUT_CONTAIN.COLUMN,
	layout_behave = LAYOUT_BEHAVE.FILL,
	color = { 0.0, 0.0, 0.0, 1.0 },
	children = { bg_star_glow }
}

return root