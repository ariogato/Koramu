-- buttonResize.lua

local buttonResize = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function buttonResize:onCreate ()
	-- Instanz des Buttons anhand seiner Id holen
	buttonResize.instance = Button.getInstance ("buttonResize")

	-- Button an die Fenstergr��e anpassen
	buttonResize:align ()
end

-- Position des Buttons an die Fenstergr��e anpassen
function buttonResize:align ()
	-- Aktuelle Fensterbreite holen
	gameWidth = TheGame:getGameWidth ()

	-- Breite und H�he des Buttons festlegen
	buttonResize.instance:setSize (60, 60)
	width, height = buttonResize.instance:getSize ()

	-- Neue Position, abh�ngig von der Fensterbreite, berechnen und festlegen
	buttonResize.instance:setPosition ((gameWidth - width), 0.0)
end

return buttonResize

