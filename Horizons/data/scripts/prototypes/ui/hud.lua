local bit = require("bit")

local item_bar = {	-- Item Bar
	layout_contain = LAYOUT_CONTAIN.ROW,
	layout_behave = bit.bor( LAYOUT_BEHAVE.BOTTOM, LAYOUT_BEHAVE.HFILL ),
	margins = { left = 30, right = 30, bottom = 20 },
	height = 70,
	color = { 0.6, 0.4, 0.8, 0.6 },
	children = {}
}

local hud_root = {
	layout_contain = LAYOUT_CONTAIN.COLUMN,
	layout_behave = LAYOUT_BEHAVE.FILL,
	children = {
		{	-- Top half.
			layout_behave = LAYOUT_BEHAVE.FILL,
			children = {
				{ prototype = "test_label" }
			},
		},
		{	-- Bottom half.
			layout_behave = LAYOUT_BEHAVE.FILL,
			children = { item_bar }
		}
	}
}

for i=1,10 do
	item_bar.children[i] = {
		layout_behave = LAYOUT_BEHAVE.CENTER,
		width = 50,
		height = 50,
		margins = { left = 10, right = 10 },
		color = { 0.3, 1.0, 0.4, 0.6 },
		texture = "test_small",
		OnClick = function(registry, entity, button)
			print("Click ", registry, entity, button)
			return true
		end
	}
end

return hud_root