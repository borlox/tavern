local MoveCompleteFuncs = {}

function AddUnit(unitType, map)
	if not map then
		map = World:GetMap("")
	end

	local unitDef = GetUnitType(unitType)
	local texture = LoadTexture(unitDef.model)
	local unit = AnimatedObject(texture)
	World:GetMap(""):AddGameObject(unit)
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

EventHandler:AddEventHandler("ObjectPathComplete", function(event, objId)
	if MoveCompleteFuncs[objId] then
		MoveCompleteFuncs[objId](objId)
	end
end)
