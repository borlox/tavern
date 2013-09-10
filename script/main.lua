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

function AddUnit(unitType)
	local unitDef = GetUnitType(unitType)
	local texture = LoadTexture(unitDef.model)
	local unit = AnimatedObject(texture)
	World:GetMap(""):AddGameObject(unit)
	return unit
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
		Log(LogLevel.Debug, "Click outside map")
		return
	end

	if arg.button == MouseButton.Left then
		local objects = map:GetMapObjects("objects")
		local handledByObject = false

		for obj in objects do
			local objpos = map:PixelToTile(Vector2f(obj:GetPosition()))
			local objdim = map:PixelToTile(Vector2f(obj:GetDimensions()))
			if PointInRect(tilePos, objpos, objdim) then
				handledByObject = HandleObjectClick(obj, tilePos)
				break
			end
		end

		if not handledByObject then
			UnitMoveTo(World:GetHero(), tilePos)
		end
	end
end)
