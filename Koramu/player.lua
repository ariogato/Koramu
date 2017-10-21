-- player.lua

local player = {}

function player:onCreate()
	player.instance = Player.getInstance ()
	print("I'm here")
end


function player:onCollision()
	--player.instance:sayhi()
end

return player