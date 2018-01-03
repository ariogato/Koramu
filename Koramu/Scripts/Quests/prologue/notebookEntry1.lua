-- notebookEntry1.lua

--[[
	notebookEntry1 ist die allererste Quest im Spiel. 
	Sie endet sobald das Notizbuch zum ersten mal geschlossen wird 
	und der Spieler sich aus seinem Haus (houseMap1) begibt.
]]--

local notebookEntry1 = {}

function notebookEntry1:onStart ()--[[	
		Der Spieler wird an seine Startposition in seinem Haus gebracht.
		Diese Position ist in der Map als Objekt festgelegt.

		Die Position des Objektes wird mit getPosition() extrahiert 
		und dient dann als neue Position des Players
	
	TheGame:enterMap ("houseMap1")
	player = Player:getInstance ()
	startPosition = SDL_GameObject:getInstance ("startPositionOwnHouse")
	x, y = startPosition:getPosition ()
	player:setPosition (x, y)
	]]--
--	Das Notizbuch wird geöffnet
--TheGame:openNotebook ()
end

function notebookEntry1:onDone()
	
end


return notebookEntry1