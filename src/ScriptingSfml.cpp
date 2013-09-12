#include "pch.h"
#include "ScriptingSfml.h"
#include "ScriptingHelper.h"

using namespace luabind;

scope ExportEventClasses()
{
	return
		class_<ExportEnumDummy<sf::Mouse::Button>>("MouseButton")
		.enum_("button") [
			value("Left", sf::Mouse::Left),
			value("Right", sf::Mouse::Right),
			value("Middle", sf::Mouse::Middle),
			value("XButton1", sf::Mouse::XButton1),
			value("XButton2", sf::Mouse::XButton2)
		],

		class_<sf::Event::MouseButtonEvent>("MouseButtonEvent")
		.def_readonly("button", &sf::Event::MouseButtonEvent::button)
		.def_readonly("x", &sf::Event::MouseButtonEvent::x)
		.def_readonly("y", &sf::Event::MouseButtonEvent::y),

		class_<sf::Event::MouseMoveEvent>("MouseMoveEvent")
		.def_readonly("x", &sf::Event::MouseMoveEvent::x)
		.def_readonly("y", &sf::Event::MouseMoveEvent::y)
	;
}

scope ExportUtilityClasses()
{
	return
		class_<sf::Vector2f>("Vector2f")
		.def(luabind::constructor<float, float>())
		.def(luabind::constructor<sf::Vector2i>())
		.def_readwrite("x", &sf::Vector2f::x)
		.def_readwrite("y", &sf::Vector2f::y),

		class_<sf::Vector2i>("Vector2i")
		.def(luabind::constructor<int, int>())
		.def(luabind::constructor<sf::Vector2f>())
		.def_readwrite("x", &sf::Vector2i::x)
		.def_readwrite("y", &sf::Vector2i::y),

		class_<sf::Texture>("Texture")
	;
}

scope ExportSfmlClasses()
{
	return
		ExportUtilityClasses(),
		ExportEventClasses()
	;
}
