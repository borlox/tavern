
Log(LogLevel.Msg, "Hello world from Lua!")

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

EventHandler:AddEventHandler("StateStart", function(event, state)
	Log(LogLevel.Msg, "State start: " .. state)
end)

local function HandleObjectClick(object)
	Log(LogLevel.Msg, "Object clicked: " .. object:GetName())
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
		local objects = map:GetObjects("objects")

		for obj in objects do
			local objpos = map:PixelToTile(Vector2f(obj:GetPosition()))
			local objdim = map:PixelToTile(Vector2f(obj:GetDimensions().x, obj:GetDimensions().y))
			Log(LogLevel.Msg, "Object: " .. obj:GetName() .. " | x = " .. objpos.x .. " | y = " .. objpos.y)
			if PointInRect(tilePos, objpos, objdim) then
				HandleObjectClick(obj)
				return
			end
		end

		local hero = World:GetHero()

		local heroPos = Vector2f(hero:GetPosition())
		local path = map:FindPath(heroPos, tilePos)
		hero:FollowPath(path)
	end
end)


function Initialize()
	local function HandleEvent(event, ...)
		EventHandler:HandleEvent(event, ...)
	end

	SetEventHandler(HandleEvent)
end

Initialize()
