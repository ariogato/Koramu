-- jenny.lua

local jenny = {dir = false}

function jenny:interact (n, p)
end

function jenny:onCollision ()
	characters = {}
	characters.jenny = NPC.getInstance ("jenny")
	characters.player = Player.getInstance ()
	if TheGame:getPartQuestId () == "talkToJenny" then
		TheGame:startDialog ("Girl: I am sorry for bumping into you, Sir.\nI was deeply in thought, my mother is very ill and I am worried about her so I decided to take a walk and...\nCornelius: Never mind. I am so glad to finally meet somebody who is less grumpy than that old clockmaker whom I left my watch for repair.",
		characters.jenny, characters.player)
		TheGame:startDialog ("Girl: He is not grumpy, he just doesn't like strangers. Don't take it personally.\nBy the way, his watches are very popular in town. Nearly everyone here owns at least one piece of his making.\nI wish I could afford one myself. ... Oh, I think I haven't introduced myself yet: I am Jenny.",
		characters.jenny, characters.player)
		TheGame:startDialog ("Cornelius: Nice to meet you, Jenny. My name is Cornelius. I've just arrived here this morning and don't know anything about this town.\nWould you mind telling me more about it?\nJenny: Well, they say the clockmaker's shop was the first building in town, but nobody knows how old he really is\nor how long he has been here. My mother has known him since she was a child.",
		characters.jenny, characters.player)
		TheGame:startDialog ("May I show you around the town?\nCornelius: I'd be glad to have a guide but I have to pick up my clock now, I'm afraid.\nWhat time is it actually?\nJenny: Just have a look at the church clock on the tower.",
		characters.jenny, characters.player)
		-- Todo: Fokus auf Kirchturmuhr setzen
		TheGame:startDialog ("Jenny: We could meeet in front of the church, if you like.\nCornelius: I will hurry up, thank you very much.",
		characters.jenny, characters.player)
		characters.jenny:moveRelative (1100, 0)
		TheGame:nextQuest()
	end
end

function jenny:onCommandDone (command)

	--	Bei der Quest "talkToJenny" soll Jenny auf und ab laufen (der Startimpuls wird bei findClockmakerShop:onDone gegeben
	if TheGame:getPartQuestId () == "talkToJenny" and command == "COMMAND_MOVE" then

		--	Der NPC mit der ID "jenny" wird gespeichert
		jen = NPC.getInstance ("jenny")

		--	Abwechselnd nach links und nach rechts gehen
		if jenny.dir then
			jen:moveRelative ((-200), 0)
		else
			jen:moveRelative (200, 0)
		end

		--	Die flag dir muss geändert werden, damit die Richtung beim nächsten command geändert wird
		jenny.dir = (not jenny.dir)
	end
end

return jenny