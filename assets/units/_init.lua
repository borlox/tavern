local Units = {}

function Answer(text, action)
	return {
		text = text,
		action = action,
	}
end

function RegisterUnitType(id, def)
	Units[id] = def
	Log(LogLevel.Debug, "Unit type '" .. id .. "' has been registered")
end

function GetUnitType(id)
	return Units[id]
end
