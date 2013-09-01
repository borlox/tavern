#ifndef SCRIPTING_H
#define SCRIPTING_H

class Scripting
{
	luabind::object updateFunc;
	luabind::object eventFunc;

	lua_State* L;

public:
	static Scripting& Get()
	{
		static Scripting instance;
		return instance;
	}

	void Initialize();

	void ExecuteFile(const boost::filesystem::path& path);

	void Update(float elapsed);
	void HandleEvent(sf::Event& event);

	void exp_SetUpdateHandler(luabind::object func);
	void exp_SetEventHandler(luabind::object func);

private:
	Scripting();

	void InitLuaState();
};

#endif //SCRIPTING_H
