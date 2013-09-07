#ifndef TILEMAP_H
#define TILEMAP_H

#include "sf_tile_layer.h"
#include "sf_object_layer.h"
#include "sf_tileset.h"

namespace sftile {
	class SfCamera;
	namespace priv {
		class TilemapLoader;
	}
}
using namespace sftile;
using namespace priv;

/// Orientation of the tile map.
enum TilemapOrientation
{
  SFTILE_ORIENT_UNSUPPORTED = -1,
  SFTILE_ORIENT_ORTHOGONAL = 0,
};

/// Encoding of the tile map data.
enum TilemapEncoding
{
  SFTILE_ENCODING_UNSUPPORTED = -1,
  SFTILE_ENCODING_XML = 0,
  SFTILE_ENCODING_BASE64,
  SFTILE_ENCODING_CSV
};

/// Compression of the tile map data.
enum TilemapCompression
{
  SFTILE_COMPRESS_NONE = 0,
  SFTILE_COMPRESS_GZIP,
  SFTILE_COMPRESS_ZLIB
};

class Tilemap
{
friend class priv::SfTilemapLoader;
public:
	typedef std::stack<sf::Vector2f> Path;

	/**
	 * \brief Default constructor
	 *
	 * Constructs an empty tile map.
	 */
	Tilemap();

	/**
	 * \brief Copy constructor
	 *
	 * Copies an existing tile map.
	 */
	Tilemap(const Tilemap& copy);

	/**
	 * \brief Move constructor
	 *
	 * Moves an existing tile map.
	 */
	Tilemap(Tilemap&& moved);

	/**
	 * \brief Assignment operator
	 *
	 * Copies an existing tile map.
	 */
	Tilemap& operator=(Tilemap copy);

	/**
	 * \brief Destructor
	 * 
	 * Destroys the tile map.
	 */
	~Tilemap();

	/**
	 * \brief Registers a camera with the tile map by saving
	 *        a pointer to the camera object.
	 * 
	 * \param _camera Pointer to the camera object
	 */
	void RegisterCamera(SfCamera* _camera);

	/**
	 * \brief Handles any events referring to the tile map.
	 *
	 * \param _evt SFML event object
	 */
	void HandleEvents(sf::Event _evt);

	/**
	 * \brief Updates anything in the tile map that needs
	 *        updating.
	 */
	void Update();

	/**
	 * \brief Renders the tile map.
	 *
	 * \param _window SFML window to render to
	 */
	void Render(sf::RenderWindow& _window);

	sf::Vector2f TileToScreen(sf::Vector2f tilePos);
	sf::Vector2f ScreenToTile(sf::Vector2f screenPos);

	bool ContainsScreenPos(sf::Vector2f screenPos)
	{
		return ContainsTilePos(ScreenToTile(screenPos));
	}

	bool ContainsTilePos(sf::Vector2f tilePos)
	{
		return tilePos.x >= 0 && tilePos.x < map_dimensions.x &&
		       tilePos.y >= 0 && tilePos.y < map_dimensions.y;
	}

	bool IsTileAccessible(sf::Vector2f tilePos)
	{
		if (!ContainsTilePos(tilePos))
			return false;

		if (!collisionLayer)
			return true;
		return collisionLayer->GetTileGID(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y)) != 0;
	}

	Path FindPath(sf::Vector2f start, sf::Vector2f end);

	static luabind::scope ExportClass()
	{
		return
			luabind::class_<Tilemap>("Tilemap")
			.def("TileToScreen", &Tilemap::TileToScreen)
			.def("ScreenToTile", &Tilemap::ScreenToTile)
			.def("ContainsScreenPos", &Tilemap::ContainsScreenPos)
			.def("ContainsTilePos", &Tilemap::ContainsTilePos)
			.def("IsTileAccessible", &Tilemap::IsTileAccessible)
			.def("FindPath", &Tilemap::FindPath)
			,
			luabind::class_<Tilemap::Path>("Path")
		;
	}

private:
	void UpdateCollisionLayer();

	sf::Vector2f FindNearestAccessible(sf::Vector2f start, sf::Vector2f);
	std::vector<std::pair<sf::Vector2i, float>> FindNeighbors(sf::Vector2i tile);

	sftile::SfCamera* camera;

	std::vector<SfTileset> tilesets;

	SfTileset& TilesetForGID(size_t gid);

	vector<SfTileLayer> tile_layers;

	SfTileLayer* collisionLayer;

	vector<SfObjectLayer> object_layers;

	/// Dimensions of the tile map in tiles
	sf::Vector2i map_dimensions;

	/// Dimensions of the tiles in the map in pixels
	sf::Vector2i tile_dimensions;

	/// Version of TMX being used
	float version;

	/// Orientation of the tile map
	TilemapOrientation orientation;

	/// Encoding of the tile map
	TilemapEncoding encoding;

	/// Compression of the tile map
	TilemapCompression compression;
};

#endif // TILEMAP_H
