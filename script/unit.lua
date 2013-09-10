local MoveCompleteFuncs = {}

function UnitMoveTo(obj, target)
	local start = Vector2f(obj:GetPosition())
	local path = World:GetMap(""):FindPath(start, target)
	obj:FollowPath(path, false)
	MoveCompleteFuncs[obj:GetId()] = nil
end

function UnitMoveToThen(obj, target, func)
	local start = Vector2f(obj:GetPosition())
	local path = World:GetMap(""):FindPath(start, target)
	obj:FollowPath(path, true)
	MoveCompleteFuncs[obj:GetId()] = func
end

EventHandler:AddEventHandler("ObjectPathComplete", function(event, objId)
	if MoveCompleteFuncs[objId] then
		MoveCompleteFuncs[objId](objId)
	end
end)
