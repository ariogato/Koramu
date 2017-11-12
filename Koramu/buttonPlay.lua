-- buttonPlay.lua

local buttonPlay = {}

function buttonPlay:onCreate ()
	buttonPlay.instance = Button.getInstance ("buttonPlay")
	
	buttonPlay:align ()
end

function buttonPlay:align ()

	gameWidth = TheGame:getGameWidth ()

	width, height = buttonPlay.instance:getSize ()
	xPos, yPos = buttonPlay.instance:getPosition ()

	buttonPlay.instance:setPosition ((gameWidth - width) / 2, yPos)

end

return buttonPlay

