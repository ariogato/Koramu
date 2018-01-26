-- ownHouseDoorExit .lua

local ownHouseDoorExit = {}

function ownHouseDoorExit:onCollision()
	TheGame:exitMap()

	if TheGame:getPartQuestId() == "prologue" then
		TheGame:nextQuest()
	end
end

return ownHouseDoorExit