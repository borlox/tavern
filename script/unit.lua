local MoveCompleteFuncs = {}

local IdToType = {}

function AddUnit(unitType, id, map)
	if not map then
		map = World:GetMap("")
	end

	local unitDef = GetUnitType(unitType)
	local texture = LoadTexture(unitDef.model)
	local unit = AnimatedObject(texture)
	World:GetMap(""):AddGameObject(unit)
	unit:SetId(id)
	IdToType[id] = unitType
	return unit
end

function UnitMoveTo(obj, target)
	local start = obj:GetPosition()
	local path = World:GetMap(""):FindPath(start, target)
	obj:FollowPath(path, false)
	MoveCompleteFuncs[obj:GetId()] = nil
end

function UnitMoveToThen(obj, target, func)
	local start = obj:GetPosition()
	local path = World:GetMap(""):FindPath(start, target)
	obj:FollowPath(path, true)
	MoveCompleteFuncs[obj:GetId()] = func
end

function GetUnitRect(obj)
	local pos = obj:GetPosition()
	pos = Vector2f(pos.x - 0.5, pos.y-0.5)
	local size = Vector2f(1, 1)

	return pos, size
end

function GetTypeId(obj)
	local id = obj:GetId()
	return IdToType[id]
end

EventHandler:AddEventHandler("ObjectPathComplete", function(event, objId)
	if MoveCompleteFuncs[objId] then
		MoveCompleteFuncs[objId](objId)
	end
end)
