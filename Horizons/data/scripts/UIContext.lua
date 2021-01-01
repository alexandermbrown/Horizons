local Class = require("data.scripts.Class")
local UIElement = require("data.scripts.UIElement")

---------------
-- UIContext --
---------------
UIContext = Class()
function UIContext:Init(registry, z_start, z_range)
	-- Used to lookup an element by name.
	self.registry = registry
	self.elements = {}
	self.id = UI.CreateUIContext(registry, z_start, z_range)
end

function UIContext:NewElement(name)
	local element = UIElement(self.registry, name)
	self.elements[name] = element
	return element
end

-- Calling Rebuild() will rebuild the element tree starting from the context root.
-- Call this when you add or remove elements from the context.
-- You do not need to call Recalculate(), the layout will be recalculated regardless.
function UIContext:Rebuild()
	UI.UIContextRebuild(self.registry, self.id)
end

-- Calling Recalculate() will simply recalculate the positions of all existing
-- elements, using the layout properties from the last Rebuild().
-- Set this to true whenever you edit a layout property of a UIElement,
-- for example SetContain, SetBehave, SetSize, SetMargins, etc.
function UIContext:Recalculate()
	UI.UIContextRecalculate(self.registry, self.id)
end

-- Sets the root element of the UIContext.
-- Must call Rebuild() after all elements have been added.
function UIContext:AddChild(child)
	-- UIContext has a ui_element component (the root) associated with its ID.
	UI.UIElementAddChild(self.registry, self.id, child.id)
end

function UIContext:GetElement(name)
	return self.elements[name]
end

return UIContext