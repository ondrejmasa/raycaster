#include "Resources.h"

std::vector<std::unique_ptr<sf::Texture>> Resources::textures;
sf::Image Resources::floorTexture;

void Resources::loadTextures(const std::vector<std::string>& aFiles)
{
	for (size_t i = 0; i < aFiles.size(); ++i)
	{
		auto t = std::make_unique<sf::Texture>();
		if (!t->loadFromFile(aFiles[i]))
		{
			throw std::runtime_error("Cannot load texture: " + aFiles[i]);
		}
		textures.push_back(std::move(t));
	}
}

void Resources::loadFloorTexture(const std::string& aFile)
{
	if (!floorTexture.loadFromFile(aFile))
	{
		throw std::runtime_error("Cannot load floor texture: " + aFile);
	}
}

sf::Texture& Resources::getTexture(const size_t aIdx)
{
	return *textures[aIdx];
}

sf::Image& Resources::getFloorTexture()
{
	return floorTexture;
}
