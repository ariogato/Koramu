-- koramu.lua

local koramu = {}

function koramu:onCreate ()

	koramu.instance = GameObject.getInstance ("koramu")
	
	koramu:align ()
end

function koramu:align ()

	gameWidth = TheGame:getGameWidth ()

	width, height = koramu.instance:getSize ()
	xPos, yPos = koramu.instance:getPosition ()

	koramu.instance:setPosition ((gameWidth - width) / 2, yPos)

end

return koramu