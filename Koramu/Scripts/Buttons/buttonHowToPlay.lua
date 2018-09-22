-- buttonHowToPlay.lua

local buttonHowToPlay = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonHowToPlay:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonHowToPlay.instance = Button.getInstance ("buttonHowtoplay")

	-- Button an die Fenstergröße anpassen
	buttonHowToPlay:align ()
end

-- Position und Größe des Buttons an die Fenstergröße anpassen
function buttonHowToPlay:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Buttons holen
	width, height = buttonHowToPlay.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	buttonHowToPlay.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	buttonHowToPlay.instance:setPosition ((gameWidth - width) / 2, 0.49 * gameHeight)
end

return buttonHowToPlay

