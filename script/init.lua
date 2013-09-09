
Log(LogLevel.Msg, "Hello world from Lua!")

function AddTestUnit()
	local tex = LoadTexture("assets/image/character/TestCharacter.png")
	local obj = AnimatedObject(tex)
	obj:SetPosition(4, 4)
	World:GetMap(""):AddGameObject(obj)
	return obj
end

local EventHandler = {
	handler = {},
}
_G.EventHandler = EventHandler

function EventHandler:AddEventHandler(event, func)
	if not self.handler[event] then
		self.handler[event] = {}
	end
	
	h = self.handler[event]
	h[#h+1] = func
end

function EventHandler:HandleEvent(event, ...)
	if self.handler[event] then
		for _, func in ipairs(self.handler[event]) do
			func(event, ...)
		end
	end
end

local Object_MoveCompleteFunc = {}

local function Object_MoveTo(obj, target)
	local start = Vector2f(obj:GetPosition())
	local path = World:GetMap(""):FindPath(start, target)
	obj:FollowPath(path, false)
	Object_MoveCompleteFunc[obj:GetId()] = nil
end

local function Object_MoveToThen(obj, target, func)
	local start = Vector2f(obj:GetPosition())
	local path = World:GetMap(""):FindPath(start, target)
	obj:FollowPath(path, true)
	Object_MoveCompleteFunc[obj:GetId()] = func
end

EventHandler:AddEventHandler("StateStart", function(event, state)
	Log(LogLevel.Msg, "State start: " .. state)
end)

EventHandler:AddEventHandler("ObjectPathComplete", function(event, objId)
	if Object_MoveCompleteFunc[objId] then
		Object_MoveCompleteFunc[objId]()
	end
end)

local function ParseOrientation(str)
	str = str:lower()
	if str == "south" then
		return Orientation.South
	elseif str == "north" then
		return Orientation.North
	elseif str == "west" then
		return Orientation.West
	elseif str == "east" then
		return Orientation.East
	end
end

local ObjectHandlers = {
	readable = function(obj)
		DisplayText(obj:GetProperty("textfile"))
	end,

	door = function(obj)
		Log(LogLevel.Msg, "Door to " .. obj:GetProperty("target"))

		local target = obj:GetProperty("target")
		local oldMapId = World:CurrentMapId()

		World:LoadMap(target)

		local newMap = World:GetMap("")
		local foundPos = false
		local ot = nil
		for obj in newMap:GetMapObjects("objects") do
			if obj:GetType() == "spawn" and obj:GetProperty("from") == oldMapId then
				local center = obj:GetCenter()
				local pos = newMap:PixelToTile(Vector2f(center.x, center.y))
				World:GetHero():SetPosition(pos.x, pos.y)
				foundPos = true

				ot = ParseOrientation(obj:GetProperty("orientation"))
			end
		end
		if not foundPos then
			World:GetHero():SetPosition(2, 2)
		end

		if ot ~= nil then
			World:GetHero():SetOrientation(ot)
		end
	end,
}

local function HandleObjectClick(object, clickPos)
	Log(LogLevel.Msg, "Object clicked: " .. object:GetName())

	local type = object:GetType()
	if ObjectHandlers[type] then
		Object_MoveToThen(World:GetHero(), clickPos, function()
			ObjectHandlers[type](object)
		end)
	end
end

local function PointInRect(pt, topleft, dim)
	return pt.x > topleft.x and pt.x <= (topleft.x + dim.x)
	   and pt.y > topleft.y and pt.y <= (topleft.y + dim.y)
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

		for obj in objects do
			local objpos = map:PixelToTile(Vector2f(obj:GetPosition()))
			local objdim = map:PixelToTile(Vector2f(obj:GetDimensions().x, obj:GetDimensions().y))
			--Log(LogLevel.Msg, "Object: " .. obj:GetName() .. " | x = " .. objpos.x .. " | y = " .. objpos.y)
			if PointInRect(tilePos, objpos, objdim) then
				HandleObjectClick(obj, tilePos)
				return
			end
		end

		--local hero = World:GetHero()

		Object_MoveTo(World:GetHero(), tilePos)

		--local heroPos = Vector2f(hero:GetPosition())
		--local path = map:FindPath(heroPos, tilePos)
		--hero:FollowPath(path)
	end
end)


function Initialize()
	local function HandleEvent(event, ...)
		EventHandler:HandleEvent(event, ...)
	end

	SetEventHandler(HandleEvent)
end

Initialize()
