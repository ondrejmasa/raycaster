#include "Resources.h"

std::vector<std::unique_ptr<sf::Texture>> Resources::textures;
sf::Image Resources::floorImage;
sf::Image Resources::ceilImage;
sf::Texture Resources::skyTexture;

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

void Resources::loadFloorImage(const std::string& aFile)
{
	if (!floorImage.loadFromFile(aFile))
	{
		throw std::runtime_error("Cannot load floor image: " + aFile);
	}
}

void Resources::loadCeilImage(const std::string& aFile)
{
	if (!ceilImage.loadFromFile(aFile))
	{
		throw std::runtime_error("Cannot load ceil image: " + aFile);
	}
}

void Resources::loadSkyTexture(const std::string& aFile)
{
	if (!skyTexture.loadFromFile(aFile))
	{
		throw std::runtime_error("Cannot load sky texture: " + aFile);
	}
}

sf::Texture& Resources::getTexture(const size_t aIdx)
{
	return *textures[aIdx];
}

sf::Image& Resources::getFloorImage()
{
	return floorImage;
}

sf::Image& Resources::getCeilImage()
{
	return ceilImage;
}

sf::Texture& Resources::getSkyTexture()
{
	return skyTexture;
}
