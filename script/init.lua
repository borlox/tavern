
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

EventHandler:AddEventHandler("MouseButtonReleased", function(event, arg)
	map = World:GetMap("")
	tilePos = map:ScreenToTile(Vector2f(arg.x, arg.y))

	if not map:ContainsTilePos(tilePos) then
		Log(LogLevel.Debug, "Click outside map")
		return
	end

	if arg.button == MouseButton.Left and map:IsTileAccessible(tilePos) then
		hero = World:GetHero()
		
		tilePos = map:ScreenToTile(Vector2f(arg.x, arg.y))
		hero:SetPosition(tilePos.x, tilePos.y)
	end
end)


function Initialize()
	local function HandleEvent(event, ...)
		EventHandler:HandleEvent(event, ...)
	end

	SetEventHandler(HandleEvent)
end

Initialize()
