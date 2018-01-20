-- findClockmakerShop.lua

local findClockmakerShop = {}

function findClockmakerShop:onStart ()
	player = {}
	player.instance = Player.getInstance ()
	TheGame:startNarrator ("How strange, the is still nobody to be seen!\nI think I will have another look around.\n Maybe I will at least find a place to get my watch fixed.", player.instance)
end


function findClockmakerShop:onDone ()
end

return findClockmakerShop