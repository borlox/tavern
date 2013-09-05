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

private:
	sftile::SfCamera* camera;

	std::vector<SfTileset> tilesets;

	SfTileset& TilesetForGID(size_t gid);

	vector<SfTileLayer> tile_layers;

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
