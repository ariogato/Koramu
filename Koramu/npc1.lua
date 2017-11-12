-- npc1.lua

local npc1 = {}

function npc1:interact (n, p)
	interact = {}
	interact.pl = p
	interact.np = n

	TheGame:startDialog ("Ich haue jetzt ab...\nDu Arsch.", interact.np, interact.pl)

	interact.np:moveRelative (500, 0)
	interact.pl:stun(1)
end

function npc1:onCommandDone (commandType)
end

return npc1