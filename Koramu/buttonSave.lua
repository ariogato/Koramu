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
	xPos, yPos = buttonSave.instance:getPosition ()

	buttonSave.instance:setPosition ((gameWidth - width) / 2, 0.45 * gameHeight)

end

return buttonSave

