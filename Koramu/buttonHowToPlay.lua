-- buttonHowToPlay.lua

local buttonHowToPlay = {}

function buttonHowToPlay:onCreate ()
	buttonHowToPlay.instance = Button.getInstance ("buttonHowtoplay")

	buttonHowToPlay:align ()
end

function buttonHowToPlay:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = buttonHowToPlay.instance:getSize ()
	xPos, yPos = buttonHowToPlay.instance:getPosition ()

	buttonHowToPlay.instance:setPosition ((gameWidth - width) / 2, 0.49 * gameHeight)

end

return buttonHowToPlay
