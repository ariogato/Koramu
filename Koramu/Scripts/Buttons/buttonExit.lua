-- buttonExit.lua

local buttonExit = {}

function buttonExit:onCreate ()
	buttonExit.instance = Button.getInstance ("buttonExit")
	
	buttonExit:align ()
end

function buttonExit:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = buttonExit.instance:getSize ()
	xPos, yPos = buttonExit.instance:getPosition ()

	buttonExit.instance:setPosition ((gameWidth - width) / 2, 0.65 * gameHeight)

end

return buttonExit

