-- buttonPlay.lua

local buttonPlay = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonPlay:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonPlay.instance = Button.getInstance ("buttonPlay")
	
	-- Button an die Fenstergröße anpassen
	buttonPlay:align ()
end

-- Position und Größe des Buttons an die Fenstergröße anpassen
function buttonPlay:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Buttons holen
	width, height = buttonPlay.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	buttonPlay.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	buttonPlay.instance:setPosition ((gameWidth - width) / 2, 0.33 * gameHeight)

end

return buttonPlay

