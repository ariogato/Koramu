-- buttonResume.lua

local buttonResume = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonResume:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonResume.instance = Button.getInstance ("buttonResume")

	-- Button an die Fenstergr��e anpassen
	buttonResume:align ()
end

-- Position und Gr��e des Buttons an die Fenstergr��e anpassen
function buttonResume:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Buttons holen
	width, height = buttonResume.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	buttonResume.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	buttonResume.instance:setPosition ((gameWidth - width) / 2, 0.29 * gameHeight)
end

return buttonResume

