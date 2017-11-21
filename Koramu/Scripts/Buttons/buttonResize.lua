-- buttonResize.lua

local buttonResize = {}

function buttonResize:onCreate ()
	buttonResize.instance = Button.getInstance ("buttonResize")

	buttonResize:align ()
end

function buttonResize:align ()

	gameWidth = TheGame:getGameWidth ()

	width, height = buttonResize.instance:getSize ()
	xPos, yPos = buttonResize.instance:getPosition ()

	buttonResize.instance:setPosition ((gameWidth - width), yPos)

end

return buttonResize

