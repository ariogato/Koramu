-- door2.lua

local door2 = {}

function door2:onCollision ()
	TheGame:exitMap ()
end

return door2