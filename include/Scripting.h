#ifndef SCRIPTING_H
#define SCRIPTING_H

// Log needed for template functions below
#include "Log.h"

class Scripting
{
	luabind::object updateFunc;
	luabind::object eventFunc;

	lua_State* L;

public:

	/**
	 * Return a reference to the Scripting instance.
	 */
	static Scripting& Get()
	{
		static Scripting instance;
		return instance;
	}

	/**
	 * Initialize the scripting system.
	 */
	void Initialize();

	/**
	 * Execute a lua file.
	 *
	 * @param path Path of the file to execute
	 * @return true if execution was successfull, false otherwise
	 */
	bool ExecuteFile(const boost::filesystem::path& path);

	/**
	 * Call the Lua defined update function.
	 *
	 * @param elapsed Time in seconds since the last call to Update
	 */
	void Update(float elapsed);

	/**
	 * Call the Lua defined event handler.
	 *
	 * @param event The event to handle
	 */
	void PostSfmlEvent(sf::Event& event);

	void PostEvent(const std::string& event);

	template <typename Arg>
	void PostEvent(const std::string& event, const Arg& arg);

	void exp_SetUpdateHandler(luabind::object func);
	void exp_SetEventHandler(luabind::object func);

private:
	Scripting();

	void InitLuaState();
};

inline void Scripting::PostEvent(const std::string& event)
{
	if (!eventFunc)
		return;

	try {
		luabind::call_function<void>(eventFunc, event);
	}
	catch (luabind::error err) {
		LOG(Error, "Error executing event handler: " << lua_tostring(L, -1));
	}
}

template <typename Arg>
inline void Scripting::PostEvent(const std::string& event, const Arg& arg)
{
	if (!eventFunc)
		return;

	try {
		luabind::call_function<void>(eventFunc, event, arg);
	}
	catch (luabind::error err) {
		LOG(Error, "Error executing event handler: " << lua_tostring(L, -1));
	}
}

#endif //SCRIPTING_H
