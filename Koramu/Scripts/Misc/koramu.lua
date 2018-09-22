-- koramu.lua

local koramu = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function koramu:onCreate ()
	-- Instanz des Objektes anhand seiner Id holen
	koramu.instance = SDL_GameObject:getInstance ("koramu")

	-- Objekt an die Fenstergr��e anpassen
	koramu:align ()
end

-- Position und Gr��e des Objektes an die Fenstergr��e anpassen
function koramu:align ()
	-- Aktuelle Breite und H�he des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Ma�e des Objektes holen
	width, height = koramu.instance:getSize ()

	-- Neue Ma�e, abh�ngig von der Fenstergr��e und einem festgelegten Seitenverh�ltnis, berechnen
	widthToHeight = 877/181
	height = (181 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Ma�e festlegen
	koramu.instance:setSize (width, height)

	-- Neue Position, abh�ngig von der Fenstergr��e, berechnen und festlegen
	koramu.instance:setPosition ((gameWidth - width) / 2, 0.035 * gameHeight)
end

return koramu