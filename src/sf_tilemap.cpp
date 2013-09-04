#include "pch.h"
#include "sf_tilemap.h"

#include "sf_camera.h"

using namespace sftile;

SfTilemap::SfTilemap()
: camera(nullptr)
, tilesets()
, tile_layers()
, object_layers()
, map_dimensions(-1, -1)
, tile_dimensions(32, 32)
, version(1.0)
{}

SfTilemap::SfTilemap(const SfTilemap& _copy)
: camera(_copy.camera)
, tilesets(_copy.tilesets)
, tile_layers(_copy.tile_layers)
, object_layers(_copy.object_layers)
, map_dimensions(_copy.map_dimensions)
, tile_dimensions(_copy.tile_dimensions)
, version(_copy.version)
{}

SfTilemap::SfTilemap(SfTilemap&& moved)
: camera(moved.camera)
, tilesets(std::move(moved.tilesets))
, tile_layers(std::move(moved.tile_layers))
, object_layers(std::move(moved.object_layers))
, map_dimensions(moved.map_dimensions)
, tile_dimensions(moved.tile_dimensions)
, version(moved.version)
{
	moved.camera = nullptr;
}

SfTilemap& SfTilemap::operator=(SfTilemap copy)
{
	using std::swap;

	swap(camera, copy.camera);
	swap(tilesets, copy.tilesets);
	swap(tile_layers, copy.tile_layers);
	swap(object_layers, copy.object_layers);
	swap(map_dimensions, copy.map_dimensions);
	swap(tile_dimensions, copy.tile_dimensions);

	return *this;
}

SfTilemap::~SfTilemap()
{
	camera = nullptr;
}

void SfTilemap::RegisterCamera(SfCamera* _camera)
{
	camera = _camera;
}

void SfTilemap::HandleEvents(sf::Event _evt)
{
	camera->HandleEvents(_evt);
}

void SfTilemap::Update()
{
	camera->Update();
}

void SfTilemap::Render(sf::RenderWindow& _window)
{
	sf::Vector2i offset = camera->GetTileOffset(tile_dimensions.x, tile_dimensions.y);
	sf::IntRect bounds = camera->GetBounds(tile_dimensions.x, tile_dimensions.y);

	for (int y = 0, tile_y = bounds.top; y < bounds.height; ++y, ++tile_y) {
		for (int x = 0, tile_x = bounds.left; x < bounds.width; ++x, ++tile_x) {

			for (unsigned int l = 0; l < tile_layers.size(); ++l) {
				if (tile_x < 0 || tile_y < 0)
					continue;
				if (tile_x >= map_dimensions.x || tile_y >= map_dimensions.y)
					continue;

				int gid = tile_layers.at(l).GetTileGID(tile_x, tile_y);
				if (gid == 0)
					continue;

				const float pos_x = static_cast<float>(x * tile_dimensions.x - offset.x);
				const float pos_y = static_cast<float>(y * tile_dimensions.y - offset.y);
				TilesetForGID(gid).RenderTile(_window, gid, pos_x, pos_y);
				//tileset.RenderTile(_window, gid, pos_x, pos_y);
			}
		}
	}
}

sf::Vector2f SfTilemap::TileToScreen(sf::Vector2f tilePos)
{
	sf::Vector2i offset = camera->GetTileOffset(tile_dimensions.x, tile_dimensions.y);
	sf::IntRect bounds = camera->GetBounds(tile_dimensions.x, tile_dimensions.y);

	float x = tilePos.x - bounds.left;
	float y = tilePos.y - bounds.top;

	float px = x * tile_dimensions.x - offset.x;
	float py = y * tile_dimensions.y - offset.y;

	return sf::Vector2f(px, py);
}
