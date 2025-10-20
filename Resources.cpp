#include "Resources.h"

std::vector<std::unique_ptr<sf::Texture>> Resources::wallTextures;
std::vector<std::unique_ptr<sf::Texture>> Resources::spriteTextures;
sf::Image Resources::floorImage;
sf::Image Resources::ceilImage;
sf::Texture Resources::skyTexture;

void Resources::loadWallTextures(const std::vector<std::string>& aFiles)
{
	for (size_t i = 0; i < aFiles.size(); ++i)
	{
		auto t = std::make_unique<sf::Texture>();
		if (!t->loadFromFile(aFiles[i]))
		{
			throw std::runtime_error("Cannot load texture: " + aFiles[i]);
		}
		wallTextures.push_back(std::move(t));
	}
}

void Resources::loadSpriteTextures(const std::vector<std::string>& aFiles)
{
	for (size_t i = 0; i < aFiles.size(); ++i)
	{
		auto t = std::make_unique<sf::Texture>();
		if (!t->loadFromFile(aFiles[i]))
		{
			throw std::runtime_error("Cannot load texture: " + aFiles[i]);
		}
		sf::Image img = t->copyToImage();
		img.createMaskFromColor(sf::Color::Black);
		t->loadFromImage(img);
		spriteTextures.push_back(std::move(t));
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

sf::Texture& Resources::getWallTexture(const size_t aIdx)
{
	return *wallTextures[aIdx];
}

sf::Texture& Resources::getSpriteTexture(const size_t aIdx)
{
	return *spriteTextures[aIdx];
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
