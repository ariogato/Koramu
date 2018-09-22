-- buttonSave.lua

local buttonSave = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonSave:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonSave.instance = Button.getInstance ("buttonSave")

	-- Button an die Fenstergr��e anpassen
	buttonSave:align ()
end

-- Position und Gr��e des Buttons an die Fenstergr��e anpassen
function buttonSave:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Buttons holen
	width, height = buttonSave.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 4.5
	height = (80 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	buttonSave.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	buttonSave.instance:setPosition ((gameWidth - width) / 2, 0.45 * gameHeight)
end

return buttonSave

