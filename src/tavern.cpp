#include "pch.h"
#include "Log.h"
#include "Scripting.h"
#include "State.h"
#include "World.h"
#include "TextureManager.h"
#include "MovableObject.h"
#include "Tilemap.h"
#include "sf_smart_camera.h"

#include "Globals.h"

World g_World;

static void InitSingletons()
{
	TextureManager::Get().Initialize();
	Scripting::Get().Initialize();
}

static void InitScripting()
{
	Scripting& s = Scripting::Get();

	s.ExecuteFile("script/init.lua");
}

int main(int argc, char **argv)
{
	Log::Logger::Instance().RedirectStdStreams();
#ifdef _DEBUG
	Log::Logger::Instance().SetLogLevel(Log::Logger::Debug);
#endif

	InitSingletons();

	sfg::SFGUI sfgui;

	sf::RenderWindow window(sf::VideoMode(800, 600), "tavern");

	sfg::Window::Ptr guiWnd(sfg::Window::Create(0));
	guiWnd->SetPosition(sf::Vector2f(0.f, 0.f));

	sfg::Button::Ptr guiQuitBtn(sfg::Button::Create("Quit"));
	guiQuitBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(sfg::Delegate([&]() {
		window.close();
	}));

	sfg::Button::Ptr guiResetBtn(sfg::Button::Create("Reset"));
	guiResetBtn->GetSignal(sfg::Widget::OnLeftClick).Connect(sfg::Delegate(std::function<void (void)>([]() {
		Scripting::Get().Reset();
		InitScripting();
	})));

	sfg::Fixed::Ptr guiFixed(sfg::Fixed::Create());
	guiFixed->Put(guiQuitBtn, sf::Vector2f(390, 6));
	guiFixed->Put(guiResetBtn, sf::Vector2f(390, 36));

	guiWnd->Add(guiFixed);
	
	window.resetGLStates();

	sf::Clock clk;

	World& world = g_World;
	auto tilemap = world.LoadTilemap("test", "maps/test.tmx");
	if (!tilemap) {
		LOG(Error, "Failed to load tilemap");
		return 0;
	}

	auto hero = new MovableObject(TextureManager::Get().Load("assets/image/character/TestCharacter.png"));
	hero->SetPosition(6, 6);
	//hero->Animate();
	world.SetHeroObject(hero);

	sftile::SfSmartCamera camera(800, 600);
	tilemap->RegisterCamera(&camera);

	SetupStates();
	InitScripting();

	State* currentState = GetState("MainMenuState");
	if (!currentState) {
		LOG(Crit, "MainMenuState not found.");
		return 0;
	}
	bool stateChanged = true;

	window.setFramerateLimit(120);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			world.HandleEvents(event);
			guiWnd->HandleEvent(event);
			Scripting::Get().PostSfmlEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		float elapsed = clk.restart().asSeconds();

		if (stateChanged) {
			currentState->Start();
			stateChanged = false;
		}

		if (!currentState->Run(elapsed)) {
			currentState->Stop();
			auto nextStateId = currentState->GetNextState();

			LOG(Msg, "Changing state from " << currentState->GetId() << " to " << nextStateId);

			currentState = GetState(nextStateId);
			if (!currentState) {
				LOG(Crit, "State " << nextStateId << " not found!");
				return 0;
			}
			stateChanged = true;
		}

		world.Update(elapsed);
		Scripting::Get().Update(elapsed);
		guiWnd->Update(elapsed);

		window.clear();
		world.Render(window);
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
