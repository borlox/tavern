
local CurrentConversation = nil

function BeginConversation(unitType)
	local def = unitType.talk._default
	local conv = unitType.talk[def]

	CurrentConversation = {
		unitType = unitType,
		cur = def,
		showOnUpdate = false,
	}

	ShowConversation(conv)
end

function ShowConversation(conv)
	CurrentConversation.showOnUpdate = false
	PrepareDialog(conv.text)
	for _, answer in ipairs(conv.answers) do
		AddDialogButton(answer.action, answer.text)
	end
	ShowDialog()
end

EventHandler:AddEventHandler("DialogButtonClicked", function(event, id)
	if not CurrentConversation then
		return
	end

	if id == "_close" then
		CurrentConversation = nil
		return
	else
		if CurrentConversation.unitType.talk[id] then
			CurrentConversation.cur = id
			CurrentConversation.showOnUpdate = true
			--ShowConversation(CurrentConversation.unitType.talk[id])
		end
	end
end)

SetUpdateHandler(function(elapsed)
	if CurrentConversation and CurrentConversation.showOnUpdate then
		ShowConversation(CurrentConversation.unitType.talk[CurrentConversation.cur])
	end
end)
