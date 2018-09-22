-- ownHouseDoorExit .lua

local ownHouseDoorExit = {}

-- Legt fest, was bei Kollision mit genau dieser einen Tür passieren soll
function ownHouseDoorExit:onCollision ()
	-- Die aktuelle Map ("ownHouse") verlassen
		TheGame:exitMap()

	-- Falls die aktuelle Queste diejenige mit der Id "prologue" ist (das ist die erste), so wird zur nächsten Quest geschaltet
	if TheGame:getPartQuestId() == "prologue" then
		TheGame:nextQuest()
	end
end

return ownHouseDoorExit