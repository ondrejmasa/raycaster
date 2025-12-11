#pragma once
#include "SFML/Graphics.hpp"


class Sprite
{
public:
	sf::Vector2f position;
	int texture;
	float scale;
	float size = 0.25f;
};

