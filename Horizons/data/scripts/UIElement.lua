local Class = require("data.scripts.Class")

local UIElement = Class()
function UIElement:Init(registry, name)
	self.name = name or "ui_element"
	self.children = {}
	self.registry = registry
	self.id = UI.CreateUIElement(registry)
end

function UIElement:SetSize(width, height)
	UI.UIElementSetSize(self.registry, self.id, width, height)
end

function UIElement:SetMargins(margins)
	UI.UIElementSetMargins(self.registry, self.id, margins)
end

-- Must call UIContext:Rebuild() or UIContext:Recalculate() to make changes visible.
function UIElement:SetContain(...)
	UI.UIElementSetContain(self.registry, self.id, ...)
end

-- Must call UIContext:Rebuild() or UIContext:Recalculate() to make changes visible.
function UIElement:SetBehave(...)
	UI.UIElementSetBehave(self.registry, self.id, ...)
end

-- Must call UIContext:Rebuild() to make changes visible.
function UIElement:AddChild(child)
	UI.UIElementAddChild(self.registry, self.id, child.id)
end

-- Set RGBA color between 0 and 1.
function UIElement:SetColor(red, green, blue, alpha)
	UI.UIElementSetColor(self.registry, self.id, red, green, blue, alpha)
end

function UIElement:SetTexture(texture)
	UI.UIElementSetTexture(self.registry, self.id, texture)
end

function UIElement:SetLabel(label_data)
	UI.UIElementSetLabel(self.registry, self.id, label_data)
end

function UIElement:AddTextureCrop()
	UI.UIElementAddTextureCrop(self.registry, self.id)
end

function UIElement:AddTextureFlicker(magnitude, delay)
	UI.UIElementAddTextureFlicker(self.registry, self.id, magnitude, delay)
end

function UIElement:SetEvents(handlers)
	UI.UIElementSetEvents(self.registry, self.id, handlers)
end

function UIElement:__tostring()
	return tostring(self.name)
end

return UIElement