
local initializedMaps = {}

local function AddUnitFromObj(obj, map)
	Log(LogLevel.Msg, "Adding unit " .. obj:GetProperty("type"))
	local type = obj:GetProperty("type")
	local ot = ParseOrientation(obj:GetProperty("orientation"))

	local unit = AddUnit(type, map)
	unit:SetPosition(map:PixelToTile(Vector2f(obj:GetCenter())))
	if ot then
		unit:SetOrientation(ot)
	end
end

function InitializeMap()
	local mapId = World:CurrentMapId()
	Log(LogLevel.Msg, "Initializing map " .. mapId)
	if initializedMaps[mapId] then
		return
	end

	local map = World:GetMap("")

	for obj in map:GetMapObjects("objects") do
		Log(LogLevel.Debug, obj:GetName() .. " " .. obj:GetType())
		if obj:GetType() == "unit" then
			AddUnitFromObj(obj, map)
		end
	end

	initializedMaps[mapId] = true
end

function SwitchMap(mapId)
	local oldMapId = World:CurrentMapId()

	World:LoadMap(mapId)

	local newMap = World:GetMap("")
	local ot = nil

	for obj in newMap:GetMapObjects("objects") do
		if obj:GetType() == "spawn" and obj:GetProperty("from") == oldMapId then
			local center = obj:GetCenter()
			local pos = newMap:PixelToTile(Vector2f(center))
			World:GetHero():SetPosition(pos)
			foundPos = true

			ot = ParseOrientation(obj:GetProperty("orientation"))
		end
	end

	if ot ~= nil then
		World:GetHero():SetOrientation(ot)
	end

	InitializeMap()
end

EventHandler:AddEventHandler("InitComplete", function(event)
	InitializeMap()
end)
