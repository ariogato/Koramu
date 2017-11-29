-- buttonSave.lua

local buttonSave = {}

function buttonSave:onCreate ()
	buttonSave.instance = Button.getInstance ("buttonSave")

	buttonSave:align ()
end

function buttonSave:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = buttonSave.instance:getSize ()
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	buttonSave.instance:setSize (width, height)
	buttonSave.instance:setPosition ((gameWidth - width) / 2, 0.45 * gameHeight)

end

return buttonSave

