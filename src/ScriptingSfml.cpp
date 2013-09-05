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
		.def_readonly("y", &sf::Event::MouseButtonEvent::y)
	;
}

scope ExportUtilityClasses()
{
	return
		class_<sf::Vector2f>("Vector2f")
		.def(luabind::constructor<float, float>())
		.def_readwrite("x", &sf::Vector2f::x)
		.def_readwrite("y", &sf::Vector2f::y)
	;
}

scope ExportSfmlClasses()
{
	return
		ExportUtilityClasses(),
		ExportEventClasses()
	;
}
