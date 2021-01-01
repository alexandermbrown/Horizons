
local function Class(base)
	local c = {}
	-- Copy base class functions.
	if type(base) == "table" then
		for i, v in pairs(base) do
			c[i] = v
		end
		c._base = base
	end

	c.__index = c

	local mt = {}
	-- Constructor function. Instantiates the class.
	function mt.__call(class_table, ...)
		local obj = {}
		setmetatable(obj, class_table)

		if class_table.Init then
			class_table.Init(obj, ...)
		elseif class_table._base and class_table._base.Init then
			class_table._base.Init(obj, ...)
		end

		return obj
	end

	function c:_InitBase(...)
		local class_table = getmetatable(self)
		if class_table._base and class_table._base.Init then
			class_table._base.Init(self, ...)
		end
	end

	function c:InheritsFrom(class)
		local class_table = getmetatable(self)
		-- iterate through all base classes.
		while class_table do
			if class_table == class then return true end
			class_table = class_table._base
		end
		return false
	end

	setmetatable(c, mt)
	return c
end

return Class