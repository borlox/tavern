#include "pch.h"
#include "Log.h"

int main(int argc, char **argv)
{
	Log::Logger::Instance().RedirectStdStreams();

	sfg::SFGUI sfgui;

	sf::RenderWindow window(sf::VideoMode(800, 600), "tavern");

	sfg::Label::Ptr testlbl(sfg::Label::Create("Hello World!"));
	sfg::Button::Ptr testbtn(sfg::Button::Create("Hello SFGUI"));

	testbtn->GetSignal(sfg::Widget::OnLeftClick).Connect(sfg::Delegate([&]() {
		testlbl->SetText("Foobar");
		testbtn->Show(false);
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

	sf::Clock clk;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			desktop.HandleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		desktop.Update(clk.restart().asSeconds());

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
