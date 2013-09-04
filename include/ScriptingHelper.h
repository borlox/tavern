#ifndef SCRIPTING_HELPER_H
#define SCRIPTING_HELPER_H

#define pick_overload(Class_, Func_, Ret_, Arg_) static_cast<Ret_ (Class_::*) Arg_>(&Class_::Func_)

template <typename T>
struct ExportEnumDummy
{ };

template <typename T>
static void GetPosition(lua_State *L, T& self)
{
	auto pos = self.GetPosition();
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
}

#endif //SCRIPTING_HELPER_H
