
function InitializeMap()

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
end
