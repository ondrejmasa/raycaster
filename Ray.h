#pragma once
#include <SFML/Graphics.hpp>
#include "global.h"

class Ray
{
private:
	sf::Vector2f positon;
	sf::Vector2f direction;
	float length;
	int wallHit;
public:
	Ray();
	const sf::Vector2f& getPositon() const;
	const sf::Vector2f& getDirection() const;
	const float& getLength() const;
	const int& getWallHit() const;
	float cast(const sf::Vector2f& aPos, const sf::Vector2f& aRayDir);
};

