-- buttonResize.lua

local buttonResize = {}

function buttonResize:onCreate ()
	buttonResize.instance = Button.getInstance ("buttonResize")

	buttonResize:align ()
end

function buttonResize:align ()

	gameWidth = TheGame:getGameWidth ()

	buttonResize.instance:setSize (60, 60)
	width, height = buttonResize.instance:getSize ()

	buttonResize.instance:setPosition ((gameWidth - width), 0.0)

end

return buttonResize

