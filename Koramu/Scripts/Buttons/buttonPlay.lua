-- buttonPlay.lua

local buttonPlay = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonPlay:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonPlay.instance = Button.getInstance ("buttonPlay")
	
	-- Button an die Fenstergr��e anpassen
	buttonPlay:align ()
end

-- Position und Gr��e des Buttons an die Fenstergr��e anpassen
function buttonPlay:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Buttons holen
	width, height = buttonPlay.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	buttonPlay.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	buttonPlay.instance:setPosition ((gameWidth - width) / 2, 0.33 * gameHeight)

end

return buttonPlay

