-- littleGirl.lua

local littleGirl = {}

function littleGirl:interact (g, p)
	interact = {}
	interact.pl = p
	interact.gi = g

	TheGame:startDialog ("I'm just an innocent little girl.\nBye Bye!", interact.gi, interact.pl)
	TheGame:showClock ()
	TheGame:addClockTimeSeconds (300, 1)
	interact.pl:stun (0.5)

	interact.gi:moveRelative (0, 400)

end

function littleGirl:onCommandDone(command)
	if command == "COMMAND_MOVE" then
		TheGame:hideClock ()
	end
end

return littleGirl