-- clockmaker.lua

local clockmaker = {}

function clockmaker:interact (n, p)
	interact = {}
	interact.pl = p
	interact.np = n
	if TheGame:getPartQuestId() == "findClockmaker" then
		TheGame:startDialog ("Cornelius: Excuse me Sir ...\nClockmaker: You don't belong here, that's clear to see. What is it you want?\nCornelius: Well my watch has stopped and I came to ask you to fix it for me.\nClockmaker: Is that so? Mmh... that is a wonderful watch. I guarantee it will work better than ever before if you give it to me.\nBut it will take some time to fix it. You can come back in half an hour at the earliest.\nAnd now don't disturb me any longer, I will go to work immediately.",
		interact.np, interact.pl)
		TheGame:nextQuest()
	end
	if TheGame:getPartQuestId () == "pickUpWatch" then
		TheGame:startDialog ("Clockmaker: There it is. Your watch works perfectly again. Take care not to ruin it again!\nAnd you should not be too curious, no good will come of it!",
		interact.np, interact.pl)
		TheGame:startDialog ("Cornelius: What do you mean?\nClockmaker: I don't have time for such talk. Off with you! And don't be late...",
		interact.np, interact.pl)
	end
end

function clockmaker:onCollision ()
end

return clockmaker