#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Resources
{
private:
	static std::vector<std::unique_ptr<sf::Texture>> textures;
	static sf::Image floorImage;
	static sf::Image ceilImage;
public:
	static void loadTextures(const std::vector<std::string>& aFiles);
	static void loadFloorTexture(const std::string& aFile);
	static void loadCeilTexture(const std::string& aFile);
	static sf::Texture& getTexture(const size_t aIdx);
	static sf::Image& getFloorImage();
	static sf::Image& getCeilImage();
};

