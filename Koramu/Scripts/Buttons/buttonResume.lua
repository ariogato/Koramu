-- buttonResume.lua

local buttonResume = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonResume:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonResume.instance = Button.getInstance ("buttonResume")

	-- Button an die Fenstergröße anpassen
	buttonResume:align ()
end

-- Position und Größe des Buttons an die Fenstergröße anpassen
function buttonResume:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Buttons holen
	width, height = buttonResume.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	buttonResume.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	buttonResume.instance:setPosition ((gameWidth - width) / 2, 0.29 * gameHeight)
end

return buttonResume

