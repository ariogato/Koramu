-- littleGirl.lua

local littleGirl = {}

function littleGirl:interact (g, p)
	interact = {}
	interact.pl = p
	interact.gi = g

	TheGame:startDialog ("I'm just an innocent little girl.\nThihihi\nBye Bye!", interact.gi, interact.pl)
	interact.pl:stun (0.5)

	interact.gi:moveRelative(0, 400)
end

return littleGirl