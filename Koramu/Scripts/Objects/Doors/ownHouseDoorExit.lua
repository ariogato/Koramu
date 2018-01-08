-- ownHouseDoorExit .lua

local ownHouseDoorExit = {}

function ownHouseDoorExit:onCollision()
	TheGame:exitMap()
end


return ownHouseDoorExit