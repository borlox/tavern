#ifndef STATE_H
#define STATE_H

class State
{
	std::string stateId;

public:
	State(const std::string& id);

	void Initialize();
	void Start();
	bool Run(float elapsed);
	void Stop();

	std::string GetNextState();

	std::string GetId() const
	{
		return stateId;
	}
};

void SetupStates();
State* GetState(const std::string& id);

#endif //STATE_H
