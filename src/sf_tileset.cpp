#include "pch.h"
#include "sf_tileset.h"

#include "tinyxml2.h"
  using namespace tinyxml2;

#include "sf_tile.h"

/// Engine namespace
namespace sftile
{

/// Private engine namespace
namespace priv
{

////////////////////////////////////////////////////////////
SfTileset::SfTileset()
: tile_dimensions(32, 32)
, tileset_image(nullptr)
, first_gid(0)
{ }

////////////////////////////////////////////////////////////
SfTileset::SfTileset(const SfTileset& _copy)
: tile_dimensions(_copy.tile_dimensions)
, tileset_image(_copy.tileset_image)
, first_gid(_copy.first_gid)
{ }

SfTileset::SfTileset(SfTileset&& moved)
: tile_dimensions(moved.tile_dimensions)
, tileset_image(moved.tileset_image)
, first_gid(moved.first_gid)
{ 
	// no need to reset the moved tileset
}

SfTileset& SfTileset::operator=(SfTileset copy)
{
	using std::swap;

	swap(tile_dimensions, copy.tile_dimensions);
	swap(tileset_image, copy.tileset_image);
	swap(first_gid, copy.first_gid);

	return *this;
}


////////////////////////////////////////////////////////////
SfTileset::~SfTileset()
{}


////////////////////////////////////////////////////////////
SfTile SfTileset::GetTile(const unsigned int gid)
{
	assert(HasTileForGID(gid));
	size_t localId = gid - first_gid;
	auto imgSize = tileset_image->getSize();
	size_t nX = imgSize.x / tile_dimensions.x;
	size_t nY = imgSize.y / tile_dimensions.y;

	size_t tilePosX = localId % nX;
	size_t tilePosY = localId / nX;

	sf::IntRect tileRect;
	tileRect.left = tilePosX * tile_dimensions.x;
	tileRect.top = tilePosY * tile_dimensions.y;
	tileRect.width = tile_dimensions.x;
	tileRect.height = tile_dimensions.y;

	return SfTile(gid, tileRect);
  //return tiles.at(_id - 1);
}

////////////////////////////////////////////////////////////
void SfTileset::RenderTile(sf::RenderWindow& _window, const unsigned int _id, const float _x, const float _y)
{
  SfTile tile = GetTile(_id);
  sf::Sprite sprite(*tileset_image, tile.GetRect());
  sprite.setPosition(_x, _y);

  _window.draw(sprite);
}

}

}