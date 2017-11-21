-- door1.lua

local door1 = {}

function door1:onCollision ()
	TheGame:enterMap ("houseMap1")
end

return door1