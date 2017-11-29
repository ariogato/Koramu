-- buttonMainMenu.lua

local buttonMainMenu = {}

function buttonMainMenu:onCreate ()
	buttonMainMenu.instance = Button.getInstance ("buttonMainMenu")

	buttonMainMenu:align ()
end

function buttonMainMenu:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = buttonMainMenu.instance:getSize ()
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	buttonMainMenu.instance:setSize (width, height)
	buttonMainMenu.instance:setPosition ((gameWidth - width) / 2, 0.62 * gameHeight)

end

return buttonMainMenu

