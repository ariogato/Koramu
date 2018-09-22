-- clockmakerHouseEntry.lua

local clockmakerHouseEntry = {}

-- Legt fest, was bei Kollision mit genau dieser einen Tür passieren soll
function clockmakerHouseEntry:onCollision ()
	-- Die Map des Uhrmacherhauses betreten
	TheGame:enterMap ("clockmakerHouse")
end

return clockmakerHouseEntry