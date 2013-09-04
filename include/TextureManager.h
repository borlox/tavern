#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Log.h"

class TextureManager
{
	std::map<std::string, std::unique_ptr<sf::Texture>> textures;

public:
	static TextureManager& Get()
	{
		static TextureManager instance;
		return instance;
	}

	void Initialize()
	{

	}

	sf::Texture *Load(const std::string& file)
	{
		if (textures.find(file) == textures.end()) {
			LoadTexture(file);
		}

		return textures[file].get();
	}

	void Reset()
	{
		textures.clear();
	}

private:
	TextureManager()
	{ }

	void LoadTexture(const std::string& file) 
	{
		std::unique_ptr<sf::Texture> tex(new sf::Texture);
		if (!tex->loadFromFile(file)) {
			LOG(Error, "Failed to load texture: " << file);
			tex.reset();
		}
		textures.emplace(file, std::move(tex));
	}
};

#endif //TEXTURE_MANAGER_H
