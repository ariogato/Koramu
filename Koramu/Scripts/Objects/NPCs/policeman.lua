-- policeman.lua

local policeman = {}

function policeman:interact (np, pl)
	--	Der default Dialog
	TheGame:startDialog("You stay safe.", np, pl)
end

return policeman
