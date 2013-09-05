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
	 * Reset the scripting system.
 	 */
	void Reset();

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

	template <typename Arg1>
	void PostEvent(const std::string& event, const Arg1& arg1);

	template <typename Arg1, typename Arg2>
	void PostEvent(const std::string& event, const Arg1& arg1, const Arg2& arg2);

	void exp_SetUpdateHandler(luabind::object func);
	void exp_SetEventHandler(luabind::object func);

private:
	Scripting();

	void InitLuaState();
	void RegisterGlobals();
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

template <typename Arg1>
inline void Scripting::PostEvent(const std::string& event, const Arg1& arg1)
{
	if (!eventFunc)
		return;

	try {
		luabind::call_function<void>(eventFunc, event, arg1);
	}
	catch (luabind::error err) {
		LOG(Error, "Error executing event handler: " << lua_tostring(L, -1));
	}
}

template <typename Arg1, typename Arg2>
inline void Scripting::PostEvent(const std::string& event, const Arg1& arg1, const Arg2& arg2)
{
	if (!eventFunc)
		return;

	try {
		luabind::call_function<void>(eventFunc, event, arg1, arg2);
	}
	catch (luabind::error err) {
		LOG(Error, "Error executing event handler: " << lua_tostring(L, -1));
	}
}

#endif //SCRIPTING_H
