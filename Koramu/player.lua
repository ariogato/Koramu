-- player.lua

local player = {}

function player:onCreate()
	player.instance = Player.getInstance ()
	print ("Aktuelle Quest: \n\t" .. TheGame.getMainQuestId () .. " : " .. TheGame.getPartQuestId ())
end


function player:onCollision()
end

return player