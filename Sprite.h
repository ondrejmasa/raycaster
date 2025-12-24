#pragma once
#include "SFML/Graphics.hpp"


class Sprite
{
public:
	sf::Vector2f position;
	int texture;
	float scale;
	float size;
	Sprite(const sf::Vector2f& aPos, const int aTex, const float aScale, const float aSize = 0.25f);
	virtual ~Sprite() = default;
	virtual void updatePos(const sf::Vector2f& aPlayerPos, const float aDeltaTime) {};
};

