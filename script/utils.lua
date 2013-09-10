---------
-- Utility functions
---------

function PointInRect(pt, topleft, dim)
	return pt.x > topleft.x and pt.x <= (topleft.x + dim.x)
	   and pt.y > topleft.y and pt.y <= (topleft.y + dim.y)
end

function ParseOrientation(str)
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
