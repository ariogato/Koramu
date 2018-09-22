-- buttonHowToPlay.lua

local buttonHowToPlay = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonHowToPlay:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonHowToPlay.instance = Button.getInstance ("buttonHowtoplay")

	-- Button an die Fenstergr��e anpassen
	buttonHowToPlay:align ()
end

-- Position und Gr��e des Buttons an die Fenstergr��e anpassen
function buttonHowToPlay:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Buttons holen
	width, height = buttonHowToPlay.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	buttonHowToPlay.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	buttonHowToPlay.instance:setPosition ((gameWidth - width) / 2, 0.49 * gameHeight)
end

return buttonHowToPlay

