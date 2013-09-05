#ifndef WORLD_H
#define WORLD_H

#include "sf_tilemap_loader.h"

class GameObject;

class Tilemap;

using namespace sftile;
using namespace priv;

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
	 * \param id Unique string ID of the tile map
	 * \param path Filepath to the TMX file
	 * 
	 * \return Pointer to the loaded tile map if successful
	 */
	Tilemap* LoadTilemap(string id, string path);

	/**
	 * \brief Return a pointer to a previously loaded 
	 *        tile map.
	 *
	 * \param id Unique string ID of the tile map
	 * \return Pointer to the tile map if successful
	 */
	Tilemap* GetTilemap(string id = "");

	/**
	 * \brief Handles any events referring to the tile engine.
	 * 
	 * \param _evt SFML event object
	 */
	void HandleEvents(sf::Event _evt);

	/**
	 * \brief Updates anything in the tile engine that needs
	 *        updating.
	 * 
	 * \param elapsed Seconds elapsed since the last call to update
	 */
	void Update(float elapsed);

	/**
	 * \brief Renders the currently visible tile map and objects.
	 *
	 * \param _window SFML window to render to
	 */
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

	/**
	 * Sets the hero object.
	 *
	 * The world takes ownership of the object.
	 */
	void SetHeroObject(GameObject* hero)
	{
		heroObject.reset(hero);
	}

	/**
	 * Return a pointer to the hero object.
	 */
	GameObject* GetHeroObject()
	{
		return heroObject.get();
	}

	static luabind::scope ExportClass()
	{
		return
			luabind::class_<World>("World")
			.def("GetHero", &World::GetHeroObject)
			//.def("GetMap", &World::GetTilemap)
		;
	}

private:
	bool LoadFromTMX(string _path);

	bool MapExists(string _id);

	SfTilemapLoader loader;

	std::map<string, Tilemap> tilemaps;

	std::string current_id; // Id of the current map

	std::unique_ptr<GameObject> heroObject;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};

#endif // WORLD_H
