-- clockmakerHouseEntry.lua

local clockmakerHouseEntry = {}

function clockmakerHouseEntry:onCollision ()
	TheGame:enterMap ("clockmakerHouse")
end


return clockmakerHouseEntry