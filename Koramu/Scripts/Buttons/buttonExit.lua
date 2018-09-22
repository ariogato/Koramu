-- buttonExit.lua

local buttonExit = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonExit:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonExit.instance = Button.getInstance ("buttonExit")
	
	-- Button an die Fenstergröße anpassen
	buttonExit:align ()
end

-- Position und Größe des Buttons an die Fenstergröße anpassen
function buttonExit:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Buttons holen
	width, height = buttonExit.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	buttonExit.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	buttonExit.instance:setPosition ((gameWidth - width) / 2, 0.65 * gameHeight)
end

return buttonExit

