#pragma once

#ifndef PCH_H
#define PCH_H

// Disable warning about too long decorated names
#pragma warning(disable:4503)

// STL
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include <vector>
#include <list>
#include <map>

#include <memory>

#include <algorithm>

// BOOST
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

// Lua
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// luabind
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/iterator_policy.hpp>

// SFML
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// SFGUI
#include <SFGUI/SFGUI.hpp>

#endif //PCH_H
