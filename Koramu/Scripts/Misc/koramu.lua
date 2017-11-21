-- koramu.lua

local koramu = {}

function koramu:onCreate ()

	koramu.instance = GameObject.getInstance ("koramu")
	
	koramu:align ()
end

function koramu:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = koramu.instance:getSize ()
	xPos, yPos = koramu.instance:getPosition ()

	koramu.instance:setPosition ((gameWidth - width) / 2, 0.035 * gameHeight)

end

return koramu