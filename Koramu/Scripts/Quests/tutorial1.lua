-- tutorial1.lua

local tutorial1 = {}

function tutorial1:onStart ()
	TheGame:addNote ("01.08.1920:\nWhoooop whoop\nLet's test this length thing and check whether I am a smart boi. Let's force a new page :) ")
	TheGame:openNotebook()
end

function tutorial1:onDone()
end

return tutorial1