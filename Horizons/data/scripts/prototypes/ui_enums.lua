-- from enum lay_box_flags in layout.h
LAYOUT_CONTAIN = {
	-- left to right
    ROW = 0x002,
    -- top to bottom
    COLUMN = 0x003,

    -- free layout
    LAYOUT = 0x000,
    -- flex model
	FLEX = 0x002,
	
    -- single-line
    NOWRAP = 0x000,
    -- multi-line, wrap left to right
    WRAP = 0x004,

    -- justify-content (start, end, center, space-between)
    -- at start of row/column
    START = 0x008,
    -- at center of row/column
	MIDDLE = 0x000,
    -- at end of row/column
    END = 0x010,
    -- insert spacing to stretch across whole row/column
    JUSTIFY = 0x018,
}

-- from enum lay_layout_flags in layout.h
LAYOUT_BEHAVE = {
	-- anchor to left item or left side of parent
	LEFT = 0x020,
	-- anchor to top item or top side of parent
	TOP = 0x040,
	-- anchor to right item or right side of parent
	RIGHT = 0x080,
	-- anchor to bottom item or bottom side of parent
	BOTTOM = 0x100,
	-- anchor to both left and right item or parent borders
	HFILL = 0x0a0,
	-- anchor to both top and bottom item or parent borders
	VFILL = 0x140,
	-- center horizontally, with left margin as offset
	HCENTER = 0x000,
	-- center vertically, with top margin as offset
	VCENTER = 0x000,
	-- center in both directions, with left/top margin as offset
	CENTER = 0x000,
	-- anchor to all four directions
	FILL = 0x1e0,
	-- When in a wrapping container, put this element on a new line. Wrapping
	-- layout code auto-inserts LAY_BREAK flags as needed. See GitHub issues for
	-- TODO related to this.
	--
	-- Drawing routines can read this via item pointers as needed after
	-- performing layout calculations.
	BREAK = 0x200
}