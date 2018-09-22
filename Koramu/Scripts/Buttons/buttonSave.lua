-- buttonSave.lua

local buttonSave = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonSave:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonSave.instance = Button.getInstance ("buttonSave")

	-- Button an die Fenstergröße anpassen
	buttonSave:align ()
end

-- Position und Größe des Buttons an die Fenstergröße anpassen
function buttonSave:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Buttons holen
	width, height = buttonSave.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	buttonSave.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	buttonSave.instance:setPosition ((gameWidth - width) / 2, 0.45 * gameHeight)
end

return buttonSave

