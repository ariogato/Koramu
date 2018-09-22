-- buttonExit.lua

local buttonExit = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonExit:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonExit.instance = Button.getInstance ("buttonExit")
	
	-- Button an die Fenstergr��e anpassen
	buttonExit:align ()
end

-- Position und Gr��e des Buttons an die Fenstergr��e anpassen
function buttonExit:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Buttons holen
	width, height = buttonExit.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 3.0
	height = (120 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	buttonExit.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	buttonExit.instance:setPosition ((gameWidth - width) / 2, 0.65 * gameHeight)
end

return buttonExit

