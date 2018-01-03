-- npc1.lua

local npc1 = {}

function npc1:interact (n, p)
	interact = {}
	interact.pl = p
	interact.np = n

	TheGame:startDialog ("Ich haue jetzt ab...\nDu Arsch.", interact.np, interact.pl)

	if TheGame:getItemCount("topHat") == 1 then
		TheGame:startDialog ("Ich haue doch nicht ab...\nDu bist naemlich im Besitz von einem Top Hat.\nDu Arsch.", interact.np, interact.pl)
	else
		interact.np:moveRelative (500, 0)
		interact.pl:stun(1)
	end

end

function npc1:onCommandDone (commandType)
end

return npc1