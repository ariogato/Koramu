local door = {}

function door:hello ()
	print ("hello from lua")
end

y = { t = function () print (2 + 3) end }

door.foo = function () print ("foo") end

return door