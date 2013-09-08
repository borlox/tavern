////////////////////////////////////////////////////////////
/// SfTileEngine: C++/SFML Tile Engine
/// Copyright (C) 2013 Tyler Petresky
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////

#include "pch.h"
#include "sf_tile_layer.h"

/// Engine namespace
namespace sftile
{

/// Private engine namespace
namespace priv
{

////////////////////////////////////////////////////////////
SfTileLayer::SfTileLayer()
  : tile_gids()
  , visible(true)
  , name()
  , layer_dimensions(-1, -1)
{}


////////////////////////////////////////////////////////////
SfTileLayer::SfTileLayer(const SfTileLayer& _copy)
  : tile_gids(_copy.tile_gids)
  , visible(_copy.visible)
  , name(_copy.name)
  , properties(_copy.properties)
  , layer_dimensions(_copy.layer_dimensions)
{
	CheckProperties();
}


////////////////////////////////////////////////////////////
SfTileLayer& SfTileLayer::operator=(const SfTileLayer& _copy)
{
  if (this != &_copy)
  {
    SfTileLayer temp(_copy);

    std::swap(tile_gids, temp.tile_gids);
	std::swap(visible, temp.visible);
	std::swap(name, temp.name);
	std::swap(properties, temp.properties);
    std::swap(layer_dimensions, temp.layer_dimensions);

	CheckProperties();
  }

  return *this;
}


////////////////////////////////////////////////////////////
int SfTileLayer::GetTileGID(const int _x, const int _y)
{
  /// The TMX tileset stores tiles startring from 1.
  /// Our vector stores them starting from 0.
  int gid = tile_gids.at(_y).at(_x);

  return gid;
}

template <typename T>
T get(const std::map<std::string, std::string>& prop, std::string key, T def)
{
	if (prop.find(key) == prop.end())
		return def;
	return boost::lexical_cast<T>(prop.at(key));
}

void SfTileLayer::CheckProperties()
{
	isCliff = get<int>(properties, "cliff", 0) != 0;
}

}

}

