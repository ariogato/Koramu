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
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	buttonExit.instance:setSize (width, height)

	buttonExit.instance:setPosition ((gameWidth - width) / 2, 0.65 * gameHeight)

end

return buttonExit

