-- buttonMainMenu.lua

local buttonMainMenu = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonMainMenu:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonMainMenu.instance = Button.getInstance ("buttonMainMenu")
	
	-- Button an die Fenstergröße anpassen
	buttonMainMenu:align ()
end

-- Position und Größe des Buttons an die Fenstergröße anpassen
function buttonMainMenu:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Buttons holen
	width, height = buttonMainMenu.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	buttonMainMenu.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	buttonMainMenu.instance:setPosition ((gameWidth - width) / 2, 0.62 * gameHeight)
end

return buttonMainMenu
