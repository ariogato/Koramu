-- ownHouseDoorEntry.lua

local ownHouseDoorEntry = {}

-- Legt fest, was bei Kollision mit genau dieser einen T�r passieren soll
function ownHouseDoorEntry:onCollision ()
	-- Die Map des Hauses des Spielcharakters betreten
	TheGame:enterMap("ownHouse")
end

return ownHouseDoorEntry