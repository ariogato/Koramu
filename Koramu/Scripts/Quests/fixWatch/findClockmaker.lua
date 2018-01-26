-- findClockmakerShop.lua

local findClockmakerShop = {}

function findClockmakerShop:onStart ()
	player = {}
	player.instance = Player.getInstance ()
	TheGame:startNarrator ("How strange, there is still nobody to be seen!\nI think I will have another look around.\nMaybe I will at least find a place to get my watch fixed.", player.instance)
end


function findClockmakerShop:onDone ()

	--	Jenny wird bewegt. Das ist der Startimpuls für das Auf-und-Ab-Gehen
	jenny = NPC.getInstance ("jenny")
	jenny:setPosition(1600, 2380)
	jenny:moveRelative(200, 0)
end

return findClockmakerShop