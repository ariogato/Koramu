-- npc1.lua

local npc1 = {}

npc1.currentCol = 0

function npc1:interact (n, p)
	interact = {}
	interact.pl = p
	interact.np = n

	interact.np:moveRelative (100, 0)
	interact.pl:moveRelative ((-100), 0)
end

return npc1