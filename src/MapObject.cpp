#include "pch.h"
#include "MapObject.h"

MapObject::MapObject()
: objectType(SF_OBJECT_TYPE_UNKNOWN)
, properties()
, name("")
, type("")
, position(0, 0)
, dimensions(0, 0)
, rotation(0.f)
, gid(-1)
, visible(true)
{ }

MapObject::MapObject(const MapObject& _copy)
: objectType(_copy.objectType)
, properties(_copy.properties)
, name(_copy.name)
, type(_copy.type)
, position(_copy.position)
, dimensions(_copy.dimensions)
, rotation(_copy.rotation)
, gid(_copy.gid)
, visible(_copy.visible)
{ }

MapObject& MapObject::operator=(const MapObject& _copy)
{
	if (this != &_copy) {
		MapObject temp(_copy);

		std::swap(objectType, temp.objectType);
		std::swap(properties, temp.properties);
		std::swap(name, temp.name);
		std::swap(type, temp.type);
		std::swap(position, temp.position);
		std::swap(dimensions, temp.dimensions);
		std::swap(rotation, temp.rotation);
		std::swap(gid, temp.gid);
		std::swap(visible, temp.visible);
	}

	return *this;
}
