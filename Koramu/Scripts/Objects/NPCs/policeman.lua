-- policeman.lua

local policeman = {}

function policeman:interact (np, pl)
	--	Der default Dialog
	TheGame:startDialog ("You stay safe...\nI used to be an adventurer like you, then I took an arrow in the knee\n", np, pl)
end

return policeman
