-- buttonPlay.lua

local buttonPlay = {}

function buttonPlay:onCreate ()
	buttonPlay.instance = Button.getInstance ("buttonPlay")
	
	buttonPlay:align ()
end

function buttonPlay:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()


	width, height = buttonPlay.instance:getSize ()
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	buttonPlay.instance:setSize (width, height)
	buttonPlay.instance:setPosition ((gameWidth - width) / 2, 0.33 * gameHeight)

end

return buttonPlay

