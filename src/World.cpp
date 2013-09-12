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
		tilemap.RegisterCamera(defaultCamera);
		tilemaps.emplace(id, std::move(tilemap));
	}

	cout << "Loaded Tilemap from path: " << path << endl;

	current_id = id;
	return GetTilemap(id);
}

void World::exp_LoadTilemap(std::string name)
{
	fs::path path = "maps";
	path /= (name + ".tmx");

	auto map = LoadTilemap(name, path.string());
	//map->RegisterCamera(defaultCamera);
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
	GetTilemap(current_id)->Update(elapsed);

	heroObject->Update(elapsed);
}

void World::Render(sf::RenderWindow& window)
{
	auto tilemap = GetTilemap(current_id);
	tilemap->RenderTiles(window);
	tilemap->RenderObjects(window, heroObject.get());

	if (showHighlight) {
		sf::RectangleShape rect;
		rect.setPosition(tilemap->TileToScreen(highlightPos));
		rect.setSize(tilemap->TileToScreenSize(highlightSize));
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color(255, 255, 0, 128));
		rect.setOutlineThickness(2.0f);
		window.draw(rect);
	}
}

bool World::MapExists(string id)
{
	return tilemaps.find(id) != tilemaps.end();
}
