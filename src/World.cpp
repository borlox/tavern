#include "pch.h"
#include "World.h"
#include "GameObject.h"

#include "Tilemap.h"
#include "sf_tileset.h"
#include "sf_tilemap_loader.h"

using namespace sftile;

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

SfTilemap* World::LoadTilemap(string id, string path)
{
	SfTilemap tilemap;
  
	// Check to make sure the SfTilemap doesn't already exists.
	// If not, then attempt to parse the data for it.
	if (!MapExists(id) && !loader.LoadTilemap(path, tilemap)) {
		cout << "Failed to load SfTilemap from path: " << path << endl;
		return nullptr;
	}
  
	tilemaps.emplace(id, std::move(tilemap));

	cout << "Loaded SfTilemap from path: " << path << endl;

	current_id = id;
	return GetTilemap(id);
}

SfTilemap* World::GetTilemap(string id)
{
	if (id.empty())
		id = current_id;

	if (tilemaps.find(id) == tilemaps.end()) {
		cout << "Failed to find SfTilemap with ID: " << id << endl;
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

	for (auto& obj: gameObjects)
		obj->Update(elapsed);

	boost::sort(gameObjects, [](const GameObject::Ptr& a, const GameObject::Ptr& b) {
		return a->GetPosition().y < b->GetPosition().y;
	});
}

void World::Render(sf::RenderWindow& window)
{
	SfTilemap* tilemap = GetTilemap(current_id);

	tilemap->Render(window);

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
}

bool World::MapExists(string id)
{
	return tilemaps.find(id) != tilemaps.end();
}
