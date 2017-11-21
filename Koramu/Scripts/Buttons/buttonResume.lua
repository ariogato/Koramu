-- buttonResume.lua

local buttonResume = {}

function buttonResume:onCreate ()
	buttonResume.instance = Button.getInstance ("buttonResume")

	buttonResume:align ()
end

function buttonResume:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = buttonResume.instance:getSize ()
	xPos, yPos = buttonResume.instance:getPosition ()

	buttonResume.instance:setPosition ((gameWidth - width) / 2, 0.29 * gameHeight)

end

return buttonResume

