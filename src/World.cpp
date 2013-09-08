#include "pch.h"
#include "World.h"
#include "GameObject.h"

#include "Tilemap.h"
#include "sf_tileset.h"
#include "sf_tilemap_loader.h"

using namespace sftile;

namespace fs = boost::filesystem;

World::World()
: current_id("null")
{
	cout << "World created" << endl;
}

World::World(const World& copy)
: loader(copy.loader)
, tilemaps(copy.tilemaps)
, current_id(copy.current_id)
{}

World& World::operator=(const World& copy)
{
	using std::swap;

	if (this != &copy) {
		World temp(copy);
		swap(loader, temp.loader);
		swap(tilemaps, temp.tilemaps);
		swap(current_id, temp.current_id);
	}

	return *this;
}

Tilemap* World::LoadTilemap(string id, string path)
{
	if (!MapExists(id)) {
		Tilemap tilemap;
		if (!loader.LoadTilemap(path, tilemap)) {
			cout << "Failed to load Tilemap from path: " << path << endl;
			return nullptr;
		}
		tilemaps.emplace(id, std::move(tilemap));
	}

	cout << "Loaded Tilemap from path: " << path << endl;

	current_id = id;
	return GetTilemap(id);
}

void World::exp_LoadTilemap(std::string name)
{
	fs::path path = "maps/";
	path /= (name + ".tmx");

	auto camera = GetTilemap()->GetCamera();
	auto map = LoadTilemap(name, path.string());
	map->RegisterCamera(camera);
}

Tilemap* World::GetTilemap(string id)
{
	if (id.empty())
		id = current_id;

	if (tilemaps.find(id) == tilemaps.end()) {
		cout << "Failed to find Tilemap with ID: " << id << endl;
		return nullptr;
	}
	else {
		return &tilemaps.at(id);
	}
}

void World::HandleEvents(sf::Event _evt)
{
	GetTilemap(current_id)->HandleEvents(_evt);
}

void World::Update(float elapsed)
{
	GetTilemap(current_id)->Update();

	heroObject->Update(elapsed);
	for (auto& obj: gameObjects)
		obj->Update(elapsed);

	boost::sort(gameObjects, [](const GameObject::Ptr& a, const GameObject::Ptr& b) {
		return a->GetPosition().y < b->GetPosition().y;
	});
}

void World::Render(sf::RenderWindow& window)
{
	Tilemap* tilemap = GetTilemap(current_id);

	// render all non-cliff layers
	tilemap->Render(window, false);

	bool heroDrawn = false;
	for (auto& obj: gameObjects) {
		obj->Render(window, tilemap->TileToScreen(obj->GetPosition()));

		if (heroObject && !heroDrawn && heroObject->GetPosition().y > obj->GetPosition().y) {
			heroObject->Render(window, tilemap->TileToScreen(heroObject->GetPosition()));
			heroDrawn = true;
		}
	}

	if (heroObject && !heroDrawn)
		heroObject->Render(window, tilemap->TileToScreen(heroObject->GetPosition()));

	// render cliff layers
	tilemap->Render(window, true);
}

bool World::MapExists(string id)
{
	return tilemaps.find(id) != tilemaps.end();
}
