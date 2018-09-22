-- player.lua
local player = {}

-- Legt fest, was passiert, wenn das Spielerobjekt instanziiert wird
function player:onCreate ()
	-- Spielerobjekt anhand seiner Id holen
	player.instance = Player.getInstance ()
	print ("Aktuelle Quest: \n\t" .. TheGame.getMainQuestId () .. " : " .. TheGame.getPartQuestId ())
end


function player:onCollision()
end

return player