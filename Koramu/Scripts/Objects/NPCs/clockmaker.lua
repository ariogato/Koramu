-- clockmaker.lua

local clockmaker = {}

function clockmaker:interact (n, p)
	interact = {}
	interact.pl = p
	interact.np = n
	if TheGame:getPartQuestId() == "findClockmaker" then
		TheGame:startDialog ("Excuse me Sir ...\nYou don't belong here, that's clear to see. What is it you want?\nWell my watch has stopped and I came to ask you to fix it for me.\nIs that so? Mmh... that is a wonderful watch. I guarantee it will work better than ever before if you give it to me. But it will take some\ntime to fix it. You can come back in half an hour at the earliest. And now don't disturb me any longer, I will go to work immediately.",
		interact.np, interact.pl)
	end
end

function clockmaker:onCollision ()
end

return clockmaker