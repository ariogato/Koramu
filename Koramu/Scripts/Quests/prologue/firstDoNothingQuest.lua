-- firstDoNothingQuest.lua
--[[
	Diese Quest ist direkt abgeschlossen, damit 
	onStart der ersten Quest aufgerufen werden kann
]]--

local firstDoNothingQuest = {}

function firstDoNothingQuest:onStart ()
end

function firstDoNothingQuest:onDone()
end

return firstDoNothingQuest
