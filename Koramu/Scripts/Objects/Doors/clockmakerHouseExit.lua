-- clockmakerHouseExit.lua

local clockmakerHouseExit = {}

-- Legt fest, was bei Kollision mit genau dieser einen T�r passieren soll
function clockmakerHouseExit:onCollision ()
	-- Die aktuelle Map ("clockmakerHouse") verlassen
	TheGame:exitMap ()
end

return clockmakerHouseExit