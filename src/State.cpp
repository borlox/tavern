#include "pch.h"
#include "State.h"
#include "Scripting.h"
#include "Log.h"

State::State(const std::string& id)
: stateId(id)
{ }

void State::Initialize()
{
	LOG(Debug, "Initializing state: " << stateId);
	Scripting::Get().PostEvent("StateInitialize", stateId);
}

void State::Start()
{
	LOG(Debug, "Starting state: " << stateId);
	Scripting::Get().PostEvent("StateStart", stateId);
}

void State::Stop()
{
	LOG(Debug, "Stopping state: " << stateId);
	Scripting::Get().PostEvent("StateStop", stateId);
}

bool State::Run(float elapsed)
{
	return true;
}

std::string State::GetNextState()
{
	return "";
}

static std::map<std::string, std::unique_ptr<State>> stateMap;

void SetupStates()
{
	std::unique_ptr<State> mainMenu(new State("MainMenuState"));
	std::unique_ptr<State> game(new State("GameState"));

	stateMap.emplace(mainMenu->GetId(), std::move(mainMenu));
	stateMap.emplace(game->GetId(), std::move(game));
}

State* GetState(const std::string& id)
{
	if (stateMap.find(id) == stateMap.end())
		return nullptr;

	return stateMap[id].get();
}
