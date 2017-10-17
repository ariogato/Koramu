-- player.lua

local player = {}

function player:onCollision()
	TheGame.setGameOver()
end

return player