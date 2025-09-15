#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Resources
{
private:
	static std::vector<std::unique_ptr<sf::Texture>> textures;
public:
	static void loadTextures(const std::vector<std::string>& aFiles);
	static sf::Texture& getTexture(const size_t aIdx);
};

