#ifndef WORLD_H
#define WORLD_H

#include "sf_tilemap_loader.h"

class GameObject;

namespace sftile {
	class SfTilemap;
}

using namespace sftile;

/**
 * \class World
 * \brief Provides a class to load and interact with
 *        your tilemaps.
 *
 * "For God so loved the world that he gave his one
 * and only son, that whoever believes in him shall not
 * perish but have eternal life." -John 3:16 (NIV)
 * 
 * This class is the overhead manager for the tile engine.
 * It provides a place to store and access all of the 
 * tilemaps that are being used. This class also takes care
 * to keep track of which tilemap is currently in use and
 * automatically draws and updates only that tilemap when
 * you call the predefined HandleEvents(), Update(), and
 * Render() methods. 
 *
 * You only need one of these objects for
 * the engine to work, but I don't see why you can't use
 * more (although I see no point).
 *
 * Proper Usage
 * \code
 * // Create World object
 * World world;
 *
 * // Load a tilemap and save it in the world.
 * // This also saves a pointer to that tilemap.
 * SfTilemap* tilemap = world.LoadTilemap("testmap", "res\\maps\\test_map.tmx");
 * if (!tilemap)
 * {
 *   cout << "Failed to load tilemap" << endl;
 *   return -1;
 * }
 *
 * while (window.isOpen())
 * {
 *   sf::Event evt;
 *   while (window.pollEvent(evt))
 *   {
 *     // Handle any events passed into the engine.
 *     world.HandleEvents(evt);
 *   }
 *   // Update the engine.
 *   world.Update();
 *
 *   window.clear();
 *
 *   // Render the engine.
 *   world.Render(&window);
 *
 *   window.display();
 * }
 * \endcode
 */

class World
{
public:
	/**
	 * \brief Default constructor
	 *
	 * Creates an empty world object.
	 */
	World();

	/**
	 * \brief Copy constructor
	 *
	 * Copies an existing world object.
	 */
	World(const World& _copy);


	/**
	 * \brief Assignment operator
	 *
	 * Copies an existing world object.
	 */
	World& operator=(const World& _copy);


	/**
	 * \brief Loads a tile map from a TMX file.
	 * 
	 * The tile map is stored as an instantiated object here
	 * in the world object. However, a pointer to that
	 * specific tile map is stored outside where the user
	 * can access it, so they can maintain control over 
	 * their tile map.
	 * 
	 * \param _id Unique string ID of the tile map
	 * \param _path Filepath to the TMX file
	 * 
	 * \return Pointer to the loaded tile map if successful
	 */
	SfTilemap* LoadTilemap(string _id, string _path);


	////////////////////////////////////////////////////////////
	/// \brief Return a pointer to a previously loaded 
	///        tile map.
	///
	/// \param _id Unique string ID of the tile map
	/// \return Pointer to the tile map if successful
	///
	////////////////////////////////////////////////////////////
	SfTilemap* GetTilemap(string _id);


	////////////////////////////////////////////////////////////
	/// \brief Handles any events referring to the tile engine.
	/// 
	/// \param _evt SFML event object
	///
	////////////////////////////////////////////////////////////
	void HandleEvents(sf::Event _evt);


	////////////////////////////////////////////////////////////
	/// \brief Updates anything in the tile engine that needs
	///        updating.
	///
	////////////////////////////////////////////////////////////
	void Update(float elapsed);


	////////////////////////////////////////////////////////////
	/// \brief Renders the currently visible tile map.
	///
	/// \param _window SFML window to render to
	///
	////////////////////////////////////////////////////////////
	void Render(sf::RenderWindow& _window);

	/**
	 * Add an object to the world.
	 *
	 * The World takes ownership of the object.
	 */
	void AddObject(GameObject* obj)
	{
		gameObjects.emplace_back(obj);
	}


private:
	////////////////////////////////////////////////////////////
	/// \brief Loads a tile map from a TMX file.
	///
	/// \param _path Filepath to the TMX file
	///
	/// \return True if loading was successful
	///
	////////////////////////////////////////////////////////////
	bool LoadFromTMX(string _path);


	////////////////////////////////////////////////////////////
	/// \brief Checks for the existence of a tile map with
	///        a specific string ID.
	///
	/// \param _id Unique string ID of the tile map
	///
	/// \return True if tile map exists
	///
	////////////////////////////////////////////////////////////
	bool MapExists(string _id);

	
	priv::SfTilemapLoader loader;      // Takes care of loading the tile maps

	std::map<string, SfTilemap> tilemaps;

	std::string current_id;                 // Id of the current map

	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

#endif // WORLD_H
