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

local HelpMessages = {}

function AddHelpMessage(obj, msg)
	HelpMessages[obj] = msg
end
AddHelpMessage(AddHelpMessage, [[AddHelpMessage(obj, msg)

Add a help message for any Lua object. The message can be printed using the Help function.
]])

function Help(obj)
	if HelpMessages[obj] then
		Log(LogLevel.Msg, HelpMessages[obj])
	end
end
AddHelpMessage(Help, [[Help(obj)

Print the help message for an object registered with AddHelpMessage to the log file.
]])

AddHelpMessage(EventHandler, [[EventHandler

The EventHandler object handles dispatching game events to their handler functions.
Use the AddEventHandler method to add an handler function.
]])

AddHelpMessage(EventHandler.AddEventHandler, [[EventHandler:AddEventHandler(event, func)

Add a function to be called when the event is triggered. The function gets the event name and all parameters.
]])

local function Initialize()
	local function HandleEvent(event, ...)
		EventHandler:HandleEvent(event, ...)
	end

	SetEventHandler(HandleEvent)

	Log(LogLevel.Msg, "initialized")
end

Initialize()
