-- ownHouseDoorEntry.lua

local ownHouseDoorEntry = {}

function ownHouseDoorEntry:onCollision()
	TheGame:enterMap("ownHouse")
end


return ownHouseDoorEntry