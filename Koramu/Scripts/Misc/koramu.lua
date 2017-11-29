-- koramu.lua

local koramu = {}

function koramu:onCreate ()

	koramu.instance = GameObject.getInstance ("koramu")
	
	koramu:align ()
end

function koramu:align ()

	gameWidth = TheGame:getGameWidth ()
	gameHeight = TheGame:getGameHeight ()

	width, height = koramu.instance:getSize ()
	widthToHeight = 877/181
	height = (181 / 896) * gameHeight
	width = widthToHeight * height

	koramu.instance:setSize (width, height)
	koramu.instance:setPosition ((gameWidth - width) / 2, 0.035 * gameHeight)

end

return koramu