local Class = require("data.scripts.Class")
local Layer = require("data.scripts.Layer")
local UIContext = require("data.scripts.UIContext")

local function CreateMenuButton(context, name, text, transition_scene)
	local button = context:NewElement(name)
	button:SetContain(UI.Contain.Column)
	button:SetBehave(UI.Behave.Top)
	button:SetSize(400, 70)
	button:SetMargins({ top = 16, bottom = 16 })
	button:SetColor(0.3, 0.5, 0.4, 0.2)

	-- TODO: make labels a lua class.
	local label = context:NewElement(name .. "_label")
	label:SetContain(UI.Contain.Column)
	label:SetBehave(UI.Behave.Left)
	label:SetSize(1, 1)
	label:SetColor(1.0, 1.0, 1.0, 0.9)
	label:SetMargins({ top = 60, left = 60 })
	label:SetLabel({
		text = text,
		pt_size = 40,
		font = "Tomorrow-Regular"
	})

	button:SetEvents({
		OnClick = function(registry, entity, button)
			if button == 1 then
				App.TransitionScene(transition_scene, true)
				return true;
			end
			return false;
		end,
		OnMouseEnter = function(registry, entity)
			button:SetColor(0.3, 0.5, 0.4, 0.3)
			return true;
		end,
		OnMouseLeave = function(registry, entity)
			button:SetColor(0.3, 0.5, 0.4, 0.2)
			return true;
		end,
	})

	button:AddChild(label)

	return button
end

local MainMenuLayer = Class(Layer)
function MainMenuLayer:Init(registry)
	self:_InitBase("MainMenuLayer", registry, {
		"RNG",
		"Flicker",
		"UIClick",
		"UIHover",
		"UILayout",
		"UISortTransforms",
		"UIRender"
	})

	self.context = UIContext(self.registry, 0.1, 0.9)

	-- Background layers.
	local bg_star_glow = self.context:NewElement("bg_star_glow")
	bg_star_glow:SetContain(UI.Contain.Column)
	bg_star_glow:SetBehave(UI.Behave.Fill)
	bg_star_glow:SetTexture("space_scene_1_star_glow")
	bg_star_glow:AddTextureCrop()
	bg_star_glow:AddTextureFlicker(0.2, 0.1)
	self.context:AddChild(bg_star_glow)

	local bg_stars = self.context:NewElement("bg_stars")
	bg_stars:SetContain(UI.Contain.Column)
	bg_stars:SetBehave(UI.Behave.Fill)
	bg_stars:SetTexture("space_scene_1_stars")
	bg_stars:AddTextureCrop()
	bg_star_glow:AddChild(bg_stars)

	local bg_nebula = self.context:NewElement("bg_nebula")
	bg_nebula:SetContain(UI.Contain.Column)
	bg_nebula:SetBehave(UI.Behave.Fill)
	bg_nebula:SetTexture("space_scene_1_nebula")
	bg_nebula:AddTextureCrop()
	bg_stars:AddChild(bg_nebula)

	-- Buttons.
	local button_play = CreateMenuButton(self.context, "button_play", "Play", "GameScene")
	bg_nebula:AddChild(button_play)

	local button_level_editor = CreateMenuButton(self.context, "button_level_editor", "Level Editor", "LevelEditorScene")
	bg_nebula:AddChild(button_level_editor)

	local button_example_video = CreateMenuButton(self.context, "button_example_video", "Video", "VideoPlayerScene")
	bg_nebula:AddChild(button_example_video)

	self.context:Rebuild()
end

return MainMenuLayer