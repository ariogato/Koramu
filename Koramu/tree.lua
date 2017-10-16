-- tree.lua

local tree = {}

function tree:hello()
	for i = 1, 10 do
		print ("hello from lua")
	end
end

tree.str = "another one bites the dust"
tree["health"] = 10000
tree.isNPC = true

tree.foo = function () print ("foo") end

local function x ()
	print ("hello from lua")
end

return tree
