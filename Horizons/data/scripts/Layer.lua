local Class = require("data.scripts.Class")

local Layer = Class()

function Layer:Init(name, registry, systems)
	self.name = name
	self.registry = registry
	self.systems = systems
end

return Layer