#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Resources
{
private:
	static std::vector<std::unique_ptr<sf::Texture>> textures;
	static sf::Image floorImage;
	static sf::Image ceilImage;
	static sf::Texture skyTexture;
public:
	static void loadTextures(const std::vector<std::string>& aFiles);
	static void loadFloorImage(const std::string& aFile);
	static void loadCeilImage(const std::string& aFile);
	static void loadSkyTexture(const std::string& aFile);
	static sf::Texture& getTexture(const size_t aIdx);
	static sf::Image& getFloorImage();
	static sf::Image& getCeilImage();
	static sf::Texture& getSkyTexture();
};

