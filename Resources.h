#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Resources
{
private:
	static std::vector<std::unique_ptr<sf::Texture>> wallTextures;
	static std::vector<std::unique_ptr<sf::Texture>> spriteTextures;
	static sf::Image floorImage;
	static sf::Image ceilImage;
	static sf::Texture skyTexture;
public:
	static void loadWallTextures(const std::vector<std::string>& aFiles);
	static void loadSpriteTextures(const std::vector<std::string>& aFiles);
	static void loadFloorImage(const std::string& aFile);
	static void loadCeilImage(const std::string& aFile);
	static void loadSkyTexture(const std::string& aFile);
	static sf::Texture& getWallTexture(const size_t aIdx);
	static sf::Texture& getSpriteTexture(const size_t aIdx);
	static sf::Image& getFloorImage();
	static sf::Image& getCeilImage();
	static sf::Texture& getSkyTexture();
};

