-- littleGirl.lua

local littleGirl = {}

-- Legt fest, was bei Interaktion des Spielers mit dem NPC passiert
function littleGirl:interact (g, p)
	interact = {}
	interact.pl = p
	interact.gi = g
	-- Dialogbox mit dem übergebenen Text öffnen
	TheGame:startDialog ("I'm just an innocent little girl.\nBye Bye!", interact.gi, interact.pl)
	-- Uhr anzegen
	TheGame:showClock ()
	-- Uhr um übergebene Sekundenzahl vorstellen
	TheGame:addClockTimeSeconds (300, 1)
	-- Spieler für 0.5 Sekunden bewegungsunfähig machen
	interact.pl:stun (0.5)

	-- "littleGirl" nach unten bewegen
	interact.gi:moveRelative (0, 400)
end

-- Legt fest, was nach Abschluss eines bestimmten Commands passieren soll
function littleGirl:onCommandDone (command)
	-- Mach dem Bewegungscommand (vgl. "littleGirl:interact") wird die Uhr wieder ausgeblendet
	if command == "COMMAND_MOVE" then
		TheGame:hideClock ()
	end
end

return littleGirl