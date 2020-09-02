require "data.scripts.prototypes.ui_enums"

local hud = require("data.scripts.prototypes.ui.hud")
local main_menu = require("data.scripts.prototypes.ui.main_menu")

local ui = {}
ui["hud"] = hud
ui["main_menu"] = main_menu

ui["test_label"] = {
	name = "test_label",
	layout_behave = bit.bor( LAYOUT_BEHAVE.BOTTOM, LAYOUT_BEHAVE.RIGHT ),
	width = 1, height = 1,
	margins = { left = 40, top = 60, bottom = 80, right = 300 },
	color = { 1.0, 1.0, 1.0, 1.0 },
	label = {
		text = "JЁff®Д",
		pt_size = 36,
		font = "Lora-Regular",
		dynamic = true,
		excess = 16
	}
}

return ui