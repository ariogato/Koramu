-- buttonPlay.lua

local buttonPlay = {}

function buttonPlay:onCreate ()
	buttonPlay.instance = Button.getInstance ("buttonPlay")
	
	buttonPlay:align ()
end

function buttonPlay:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	buttonPlay.instance:setSize (300, 80)

	width, height = buttonPlay.instance:getSize ()
	xPos, yPos = buttonPlay.instance:getPosition ()

	buttonPlay.instance:setPosition ((gameWidth - width) / 2, 0.33 * gameHeight)

end

return buttonPlay

