----------
-- Main script for tavern
----------

function AddTestUnit()
	local tex = LoadTexture("assets/image/character/maid.png")
	local obj = AnimatedObject(tex)
	obj:SetPosition(4, 4)
	World:GetMap(""):AddGameObject(obj)
	return obj
end

EventHandler:AddEventHandler("StateStart", function(event, state)
	Log(LogLevel.Msg, "State start: " .. state)
end)

local ObjectHandlers = {
	readable = function(obj)
		DisplayText(obj:GetProperty("textfile"))
	end,

	door = function(obj)
		Log(LogLevel.Msg, "Door to " .. obj:GetProperty("target"))

		local target = obj:GetProperty("target")
		SwitchMap(target)
	end,
}

local function HandleObjectClick(object, clickPos)
	Log(LogLevel.Msg, "Object clicked: " .. object:GetName())

	local type = object:GetType()
	if ObjectHandlers[type] then
		UnitMoveToThen(World:GetHero(), clickPos, function()
			ObjectHandlers[type](object)
		end)
		return true
	end
	return false
end

EventHandler:AddEventHandler("MouseButtonReleased", function(event, arg)
	local map = World:GetMap("")
	local tilePos = map:ScreenToTile(Vector2f(arg.x, arg.y))

	if not map:ContainsTilePos(tilePos) then
		return
	end

	if arg.button == MouseButton.Left then
		local clickHandled = false

		-- check for clicks on map objects like doors
		for obj in map:GetMapObjects("objects") do
			local objpos = map:PixelToTile(Vector2f(obj:GetPosition()))
			local objdim = map:PixelToTile(Vector2f(obj:GetDimensions()))
			if PointInRect(tilePos, objpos, objdim) then
				clickHandled = HandleObjectClick(obj, tilePos)
				break
			end
		end
		if clickHandled then
			return
		end

		-- check for clicks on other units
		for obj in map:GetGameObjects() do

		end
		if clickHandled then
			return
		end

		-- default action on click
		UnitMoveTo(World:GetHero(), tilePos)
	end
end)
