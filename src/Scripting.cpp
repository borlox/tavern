#include "pch.h"
#include "Scripting.h"
#include "Globals.h"

using namespace luabind;
namespace fs = boost::filesystem;

static const char* REMOVED_GLOBALS[] = {
	"print", "load", "loadfile", "dofile", LUA_COLIBNAME,
};

Scripting::Scripting()
: L(nullptr)
{ }

void Scripting::Reset()
{
	eventFunc = object();
	updateFunc = object();
	lua_close(L);
	L = nullptr;

	InitLuaState();
}

void Scripting::Initialize()
{
	InitLuaState();
}

bool Scripting::ExecuteFile(const fs::path& path)
{
	int res = luaL_dofile(L, path.string().c_str());
	if (res != 0)
		LOG(Error, "Failed to execute file '" << path.string() << "': " << lua_tostring(L, -1));

	return res == 0;
}

bool Scripting::ExecuteString(const std::string& str)
{
	int res = luaL_dostring(L, str.c_str());
	if (res != 0)
		LOG(Error, "Failed to execute string: " << lua_tostring(L, -1));

	return res == 0;
}

void Scripting::Update(float elapsed)
{
	if (!updateFunc)
		return;

	try {
		call_function<void>(updateFunc);
	}
	catch (error err) {
		LOG(Error, "Error executing update function: " << lua_tostring(L, -1));
	}
}

void Scripting::PostSfmlEvent(sf::Event& event)
{
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		PostEvent("MouseButtonPressed", event.mouseButton);
		break;
	case sf::Event::MouseButtonReleased:
		PostEvent("MouseButtonReleased", event.mouseButton);
		break;
	}
}

void Scripting::exp_SetUpdateHandler(object func)
{
	updateFunc = func;
}

void Scripting::exp_SetEventHandler(object func)
{
	eventFunc = func;
}

#include "ScriptingSfml.h"
#include "GameObject.h"
#include "World.h"
#include "Tilemap.h"
#include "MapObject.h"

namespace Exp
{
	void Log(Log::Logger::LogLevel lvl, std::string str)
	{
		Log::Logger& logger = Log::Logger::Instance();
		if (lvl <= logger.GetLogLevel()) {
			logger.GetStream(lvl) << "Lua: " << str << std::endl;
		}
	}

	void SetEventHandler(object func)
	{
		Scripting::Get().exp_SetEventHandler(func);
	}

	void SetUpdateHandler(luabind::object func)
	{
		Scripting::Get().exp_SetUpdateHandler(func);
	}

	namespace DummyClass {
		struct LogLevel {};
	}
}

void Scripting::InitLuaState()
{
	L = luaL_newstate();

	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);

	open(L);

	for (auto& name: REMOVED_GLOBALS) {
		globals(L)[name] =  nil;
	}

	module(L) [
		def("Log", &Exp::Log),
		def("SetEventHandler", &Exp::SetEventHandler),
		def("SetUpdateHandler", &Exp::SetUpdateHandler),

		class_<Exp::DummyClass::LogLevel>("LogLevel")
		.enum_("loglevels")[
			value("Crit", Log::Logger::Crit),
			value("Error", Log::Logger::Error),
			value("Warning", Log::Logger::Warning),
			value("Msg", Log::Logger::Msg),
			value("Debug", Log::Logger::Debug),
			value("Trace", Log::Logger::Trace)
		],

		def("DisplayTextFile", DisplayTextFromFile),

		ExportSfmlClasses(),

		MapObject::ExportClass(),
		Tilemap::ExportClass(),
		World::ExportClass(),
		GameObject::ExportClass()
	];

	RegisterGlobals();
}

void Scripting::RegisterGlobals()
{
	globals(L)["World"] = &g_World;
}
