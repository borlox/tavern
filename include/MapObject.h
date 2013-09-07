#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

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
  ////////////////////////////////////////////////////////////
  /// \brief Default constructor
  ///
  /// Creates an empty object.
  ///
  ////////////////////////////////////////////////////////////
	MapObject();


  ////////////////////////////////////////////////////////////
  /// \brief Copy constructor
  ///
  /// Copies an existing object.
  ///
  ////////////////////////////////////////////////////////////
	MapObject(const MapObject& _copy);

  ////////////////////////////////////////////////////////////
  /// \brief Assignment operator
  ///
  /// Copies an existing object.
  ///
  ////////////////////////////////////////////////////////////
	MapObject& operator=(const MapObject& _copy);

  ////////////////////////////////////////////////////////////
  /// \brief Returns the name of the object
  ///
  /// \return String of the name of the object
  ///
  ////////////////////////////////////////////////////////////
	std::string GetName()
	{
		return name;
	}

  ////////////////////////////////////////////////////////////
  /// \brief Returns the string type of the object
  ///
  /// \return String type of the object
  ///
  ////////////////////////////////////////////////////////////
	std::string GetType()
	{
		return type;
	}


  ////////////////////////////////////////////////////////////
  /// \brief Returns the position of the object
  ///
  /// \return Position of the object in pixels
  ///
  ////////////////////////////////////////////////////////////
	sf::Vector2i GetPosition()
	{
		return position;
	}


  ////////////////////////////////////////////////////////////
  /// \brief Returns the dimensions of the object
  ///
  /// \return Dimensions of the object in pixels
  ///
  ////////////////////////////////////////////////////////////
	sf::Vector2i GetDimensions()
	{
		return dimensions;
	}


  ////////////////////////////////////////////////////////////
  /// \brief Returns the rotation angle of the object
  ///
  /// \return Rotation angle in degrees
  ///
  ////////////////////////////////////////////////////////////
	float GetRotation()
	{
		return rotation;
	}


  ////////////////////////////////////////////////////////////
  /// \brief Returns the GID of the tile being used (if any)
  ///
  /// \return GID of the tile being used (if any)
  ///
  ////////////////////////////////////////////////////////////
	int GetGid()
	{
		return gid;
	}


  ////////////////////////////////////////////////////////////
  /// \brief Returns whether the object is visible or not
  ///
  /// \return True if the object is visible
  ///
  ////////////////////////////////////////////////////////////
	bool IsVisible() const
	{
		return visible;
	}


  ////////////////////////////////////////////////////////////
  /// \brief Renders the object to the window
  ///
  /// \param _window Window to render to
  ///
  ////////////////////////////////////////////////////////////
	void Render(sf::RenderWindow& _window);

private:
  /// Type of the object
  ObjectType objectType;

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
