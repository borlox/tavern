#include "pch.h"
#include "Log.h"
#include "Scripting.h"

int main(int argc, char **argv)
{
	Log::Logger::Instance().RedirectStdStreams();

	sfg::SFGUI sfgui;

	sf::RenderWindow window(sf::VideoMode(800, 600), "tavern");

	sfg::Label::Ptr testlbl(sfg::Label::Create("Hello World!"));
	sfg::Button::Ptr testbtn(sfg::Button::Create("Quit"));

	testbtn->GetSignal(sfg::Widget::OnLeftClick).Connect(sfg::Delegate([&]() {
		window.close();
	}));

	sfg::Box::Ptr boxlayout(sfg::Box::Create(sfg::Box::VERTICAL, 5.f));
	boxlayout->Pack(testlbl);
	boxlayout->Pack(testbtn);

	sfg::Window::Ptr testwnd(sfg::Window::Create());
	testwnd->SetTitle("tavern - SFGUI");
	testwnd->Add(boxlayout);

	sfg::Desktop desktop;
	desktop.Add(testwnd);

	window.resetGLStates();

	Scripting::Get().Initialize();
	Scripting::Get().ExecuteFile("script/init.lua");

	sf::Clock clk;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			desktop.HandleEvent(event);
			Scripting::Get().HandleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		float elapsed = clk.restart().asSeconds();

		Scripting::Get().Update(elapsed);
		desktop.Update(elapsed);

		window.clear();
		sfgui.Display(window);
		window.display();
	}

	return 0;
}

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}

#endif
