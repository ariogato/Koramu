-- prologue.lua

--[[
	"prologue" ist die allererste Quest im Spiel. 
	Sie endet sobald der Spieler sich aus seinem Haus (ownHouse) begibt.
]]--

local prologue = {}

-- Legt fest, was passiert, sobald diese Quest aktiv wird
function prologue:onStart ()
	-- Übergebenen Text zum Notizbuch hinzufügen. Dieser Text erklärt dem Benutzer die Situation, in der das Spiel startet
	TheGame:addNote ("It has been two days since I have left London in order to visit my uncle's estate in Sherham. He died at young age - for unknown reasons - having left no heir")
	TheGame:addNote ("but me to take care of his properties. However, I don't consider the possibility of leading a peaceful and quiet life at my uncle's estate worth giving up my occupation as")
	TheGame:addNote ("a detective for Scotland Yard. For the small, sleepy town his villa is located in, is way too quiet for my taste. Since my arival at Sherham in the morning I have not met a single")
	TheGame:addNote ("person. Even more disturbing is the fact, that in the very moment my coach passed the first building of this town, my watch stopped. Although I am not the")
	TheGame:addNote ("superstitious kind of man, I still feel that behind those closed curtains and locked doos something is going terribly wrong.")
	--	Das Notizbuch wird geöffnet
	TheGame:openNotebook ()
end

-- Legt fest, was passiert, sobald die Quest erfüllt wird
function prologue:onDone ()

end

return prologue