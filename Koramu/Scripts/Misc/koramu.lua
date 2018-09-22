-- koramu.lua

local koramu = {}

-- Legt fest, was passiert, wenn das Objekt instanziiert wird
function koramu:onCreate ()
	-- Instanz des Objektes anhand seiner Id holen
	koramu.instance = SDL_GameObject:getInstance ("koramu")

	-- Objekt an die Fenstergröße anpassen
	koramu:align ()
end

-- Position und Größe des Objektes an die Fenstergröße anpassen
function koramu:align ()
	-- Aktuelle Breite und Höhe des Fensters holen
	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	-- Aktuelle Maße des Objektes holen
	width, height = koramu.instance:getSize ()

	-- Neue Maße, abhängig von der Fenstergröße und einem festgelegten Seitenverhältnis, berechnen
	widthToHeight = 877/181
	height = (181 / 896) * gameHeight
	width = widthToHeight * height

	-- Neue Maße festlegen
	koramu.instance:setSize (width, height)

	-- Neue Position, abhängig von der Fenstergröße, berechnen und festlegen
	koramu.instance:setPosition ((gameWidth - width) / 2, 0.035 * gameHeight)
end

return koramu