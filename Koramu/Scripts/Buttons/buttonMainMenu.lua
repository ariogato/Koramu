-- buttonMainMenu.lua

local buttonMainMenu = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonMainMenu:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonMainMenu.instance = Button.getInstance ("buttonMainMenu")
	
	-- Button an die Fenstergr��e anpassen
	buttonMainMenu:align ()
end

-- Position und Gr��e des Buttons an die Fenstergr��e anpassen
function buttonMainMenu:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Buttons holen
	width, height = buttonMainMenu.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	buttonMainMenu.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	buttonMainMenu.instance:setPosition ((gameWidth - width) / 2, 0.62 * gameHeight)
end

return buttonMainMenu
