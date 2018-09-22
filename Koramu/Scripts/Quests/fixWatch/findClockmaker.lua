-- findClockmakerShop.lua

--[[
	Zweit Quest im Spiel. 
	Die Quest "findClockmaker" wird erfüllt, sobald der Spieler das erste mal mit dem Uhrmacher interagiert.
]]--

local findClockmakerShop = {}

-- Legt fest, was passiert, sobald diese Quest aktiv wird
function findClockmakerShop:onStart ()
	-- Instanz des Spielerobjektes holen
	player = {}
	player.instance = Player.getInstance ()
	-- Textbox mit dem angegebenen Text öffnen
	TheGame:startNarrator ("How strange, there is still nobody to be seen!\nI think I will have another look around.\nMaybe I will at least find a place to get my watch fixed.", player.instance)
end

-- Legt fest, was passiert, sobald die Quest erfüllt wird
function findClockmakerShop:onDone ()
	-- Jenny wird bewegt. Das ist der Startimpuls für das Auf-und-Ab-Gehen
	jenny = NPC.getInstance ("jenny")
	jenny:setPosition(1600, 2380)
	jenny:moveRelative(200, 0)
end

return findClockmakerShop