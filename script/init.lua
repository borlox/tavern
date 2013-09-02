
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

function Initialize()
	local function HandleEvent(event, ...)
		EventHandler:HandleEvent(event, ...)
	end

	SetEventHandler(HandleEvent)
end

Initialize()
