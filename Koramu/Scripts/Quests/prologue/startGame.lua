-- startGame.lua
--[[
	Diese Quest ist direkt abgeschlossen, damit 
	onStart der ersten Quest aufgerufen werden kann.
	Es wird lediglich die Map des eigenen hauses aufgestapelt und 1 Item mit der Id "watchBroken" zum Inventar hinzugefügt.
]]--

local startGame = {}

function startGame:onStart ()
	TheGame:enterMap ("ownHouse")
	TheGame:addItem ("watchBroken", 1)
	TheGame:nextQuest ()
end

return startGame