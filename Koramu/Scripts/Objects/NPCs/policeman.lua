-- policeman.lua

local policeman = {}

-- Legt fest, was bei Interaktion des Spielers mit dem NPC passiert
function policeman:interact (np, pl)
	--	Der default Dialog
	TheGame:startDialog ("You stay safe...\nI used to be an adventurer like you, then I took an arrow in the knee\n", np, pl)
end

return policeman
