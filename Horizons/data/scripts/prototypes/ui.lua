require "data.scripts.prototypes.ui_enums"

local hud = require("data.scripts.prototypes.ui.hud")

local ui = {}
ui["hud"] = hud

ui["test_label"] = {
	layout_behave = bit.bor( LAYOUT_BEHAVE.LEFT, LAYOUT_BEHAVE.TOP ),
	width = 1, height = 1,
	margins = { left = 40, top = 60 },
	color = { 1.0, 1.0, 1.0, 1.0 },
	label = {
		text = "JЁff®",
		--id = "" -- TODO: make locale text available.
		pt_size = 36,
		font = "Lora-Regular"
	}
}

return ui