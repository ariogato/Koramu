-- npc1.lua

local npc1 = {}

npc1.currentCol = 0

function npc1:interact (n, p)
	interact = {}
	interact.pl = p
	interact.np = n

	TheGame:startDialog ("Ich haue jetzt ab...\nDu Arsch.", interact.np, interact.pl)

	interact.np:moveRelative (2000, 0)
	interact.pl:stun(1)
end

return npc1