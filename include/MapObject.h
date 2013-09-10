#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "ScriptingHelper.h"

enum ObjectType
{
  SF_OBJECT_TYPE_UNKNOWN = 0,
  SF_OBJECT_TYPE_TILE,
  SF_OBJECT_TYPE_RECT,
  SF_OBJECT_TYPE_ELLIPSE,
  SF_OBJECT_TYPE_POLYGON,
  SF_OBJECT_TYPE_POLYLINE,
  SF_OBJECT_TYPE_IMAGE
};

/// Private engine namespace
namespace sftile { namespace priv {
	class SfTilemapLoader;
}}

using namespace sftile;
using namespace priv;

class MapObject
{
	friend class priv::SfTilemapLoader;
public:
	MapObject();
	MapObject(const MapObject& _copy);
	MapObject& operator=(const MapObject& _copy);

	/**
	 * Return the name of the object
	 */
	std::string GetName()
	{
		return name;
	}

	/**
	 * Return the string type of the object
	 */
	std::string GetType()
	{
		return type;
	}

	/**
	 * Return the position of the object
	 */
	sf::Vector2i GetPosition()
	{
		return position;
	}
	
	/**
	 * Return the center position of the object
	 */
	sf::Vector2i GetCenter()
	{
		return position + sf::Vector2i(dimensions.x / 2, dimensions.y / 2);
	}

	/**
	 * Return the dimensions of the object
	 */
	sf::Vector2i GetDimensions()
	{
		return dimensions;
	}

	/**
	 * Return the rotation angle of the object
	 */
	float GetRotation()
	{
		return rotation;
	}

	/**
	 * Return the GID of the tile being used.
	 * 
	 * If no tile is used, return -1.
	 */
	int GetGid()
	{
		return gid;
	}

	/**
	 * Return whether the object is visible or not
	 */
	bool IsVisible() const
	{
		return visible;
	}

	/**
	 * Return the value of a property.
	 *
	 * If the property is not set, "" is returned.
	 *
	 * @param name The property name
	 */
	std::string GetProperty(const std::string& name) const
	{
		if (properties.find(name) != properties.end())
			return properties.at(name);
		return "";
	}

	/**
	 * Render the object to the window
	 *
	 * @param window Window to render to
	 */
	void Render(sf::RenderWindow& window);

	static luabind::scope ExportClass()
	{
		return
			luabind::class_<MapObject>("MapObject")
			.def("GetName", &MapObject::GetName)
			.def("GetType", &MapObject::GetType)
			.def("IsVisible", &MapObject::IsVisible)
			.def("GetPosition", &MapObject::GetPosition)
			.def("GetDimensions", &MapObject::GetDimensions)
			.def("GetCenter", &MapObject::GetCenter)
			.def("GetProperty", &MapObject::GetProperty)
		;
	}

private:
  /// Type of the object
  ObjectType objectType;

  std::map<std::string, std::string> properties;

  /// Name of the object
  std::string name;

  /// Custom string literal type - arbitrary string for end-user use
  std::string type;

  /// Position of the object (in pixels)
  sf::Vector2i position;

  /// Dimensions of the object(in pixels)
  sf::Vector2i dimensions;

  /// Rotation in degrees
  float rotation;

  /// GID of the tile being used (if any)
  int gid;
  
  /// Visibility
  bool visible;
};

#endif /// MAP_OBJECT_H
