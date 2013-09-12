#include "pch.h"
#include "Log.h"
#include "Scripting.h"
#include "State.h"
#include "World.h"
#include "TextureManager.h"
#include "MovableObject.h"
#include "Tilemap.h"
#include "sf_smart_camera.h"
#include "UserInterface.h"
#include "UnitCamera.h"

#include "Globals.h"

World g_World;

static void InitSingletons()
{
	TextureManager::Get().Initialize();
	UserInterface::Get().Initialize();
	Scripting::Get().Initialize();
}

void DisplayTextFromFile(const std::string& path)
{ }

int main(int argc, char **argv)
{
	Log::Logger::Instance().RedirectStdStreams();
#ifdef _DEBUG
	Log::Logger::Instance().SetLogLevel(Log::Logger::Debug);
#endif

	InitSingletons();

	sf::RenderWindow window(sf::VideoMode(800, 600), "tavern");

	window.resetGLStates();

	World& world = g_World;

	auto hero = new MovableObject(TextureManager::Get().Load("assets/image/character/TestCharacter.png"));
	hero->SetId("hero");
	hero->SetPosition(6, 6);
	world.SetHeroObject(hero);

	UnitCamera camera(hero, 32, 32, 700, 400);
	camera.SetRenderOffset(sf::Vector2f(50, 100));
	world.SetDefaultCamera(&camera);

	auto tilemap = world.LoadTilemap("test", "maps/test.tmx");
	if (!tilemap) {
		LOG(Error, "Failed to load tilemap");
		return 0;
	}

	SetupStates();

	State* currentState = GetState("MainMenuState");
	if (!currentState) {
		LOG(Crit, "MainMenuState not found.");
		return 0;
	}
	bool stateChanged = true;

	UserInterface& gui = UserInterface::Get();
	Scripting& scripting = Scripting::Get();

	scripting.PostEvent("InitComplete");

	window.setFramerateLimit(120);
	sf::Clock clk;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			gui.HandleEvent(event);

			if (!gui.PopupVisible()) {
				world.HandleEvents(event);
				Scripting::Get().PostSfmlEvent(event);
			}

			if (event.type == sf::Event::Closed)
				window.close();
			if (gui.ShouldQuit())
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
		gui.Update(elapsed);

		window.clear();
		world.Render(window);
		gui.Render(window);
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
