#ifndef WORLD_H
#define WORLD_H

#include "sf_tilemap_loader.h"

class GameObject;

class Tilemap;

namespace sftile {
	class SfCamera;
}

using namespace sftile;
using namespace priv;

class World
{
public:
	World();
	World(const World& _copy);
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

	void SetDefaultCamera(SfCamera* camera)
	{
		defaultCamera = camera;
	}

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
	 * Sets the hero object.
	 *
	 * The world takes ownership of the object.
	 */
	void SetHeroObject(GameObject* hero)
	{
		heroObject.reset(hero);
	}

	std::string CurrentMapId() const
	{
		return current_id;
	}

	/**
	 * Return a pointer to the hero object.
	 */
	GameObject* GetHeroObject()
	{
		return heroObject.get();
	}

	void exp_LoadTilemap(std::string name);

	static luabind::scope ExportClass()
	{
		return
			luabind::class_<World>("World")
			.def("GetHero", &World::GetHeroObject)
			.def("GetMap", &World::GetTilemap)
			.def("CurrentMapId", &World::CurrentMapId)
			.def("LoadMap", &World::exp_LoadTilemap)
		;
	}

private:
	bool LoadFromTMX(string _path);

	bool MapExists(string _id);

	SfTilemapLoader loader;

	SfCamera* defaultCamera;

	std::map<string, Tilemap> tilemaps;

	std::string current_id; // Id of the current map

	std::unique_ptr<GameObject> heroObject;
};

#endif // WORLD_H
