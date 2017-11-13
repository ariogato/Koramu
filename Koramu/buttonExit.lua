-- buttonExit.lua

local buttonExit = {}

function buttonExit:onCreate ()
	buttonExit.instance = Button.getInstance ("buttonExit")
	
	buttonExit:align ()
end

function buttonExit:align ()

	gameWidth = TheGame:getGameWidth ()

	width, height = buttonExit.instance:getSize ()
	xPos, yPos = buttonExit.instance:getPosition ()

	buttonExit.instance:setPosition ((gameWidth - width) / 2, yPos)

end

return buttonExit

