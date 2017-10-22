-- npc1.lua

local npc1 = {}

npc1.currentCol = 0

function npc1:interact (p, n)
	interact = {}
	interact.pl = p
	interact.np = n

	--interact.pl:sayhi ()
	--interact.np:sayhi ()

	self.currentCol = self.currentCol + 1 
	self.currentCol = self.currentCol % 6

	interact.pl:setCurrentCol (self.currentCol)
end

return npc1