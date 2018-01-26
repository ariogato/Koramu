-- clockmakerHouseExit.lua

local clockmakerHouseExit = {}

function clockmakerHouseExit:onCollision ()
	TheGame:exitMap ()
end


return clockmakerHouseExit