-- startGame.lua
--[[
	Diese Quest ist direkt abgeschlossen, damit 
	onStart der ersten Quest aufgerufen werden kann.
	Es wird lediglich die Map des eigenen hauses aufgestapelt.
]]--

local startGame = {}

function startGame:onGameStart ()
	TheGame:enterMap ("ownHouse")
	TheGame:nextQuest ()
end

return startGame