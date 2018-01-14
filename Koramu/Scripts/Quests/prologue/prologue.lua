-- prologue.lua

--[[
	prologue ist die allererste Quest im Spiel. 
	Sie endet sobald der Spieler sich aus seinem Haus (ownHouse) begibt.
]]--

local prologue = {}

function prologue:onStart ()
	TheGame:addNote ("It has been two days since I have left London in order to visit my uncle's estate in X. He died at young age - for unknown reasons - having left no heir but me to take")
	TheGame:addNote ("care of his properties. However, I don't consider the possibility of leading a peaceful and quiet life at my uncle's estate worth giving up my occupation as a detective for Scotland")
	TheGame:addNote ("Yard. For the small, sleepy town his villa is located in, is way too quiet for my taste. Since my arival at X in the morning I have not met a single person. Even more")
	TheGame:addNote ("disturbing is the fact, that in the very moment my coach passed the first building of this town, my watch stopped. Although I am not the superstitious kind of man,")
	TheGame:addNote ("I still feel that behind those closed curtains and locked doos something is going terribly wrong.")
--	Das Notizbuch wird geöffnet
	TheGame:openNotebook ()
end


function prologue:onDone ()

end

return prologue