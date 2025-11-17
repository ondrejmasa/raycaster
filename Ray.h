#pragma once
#include <SFML/Graphics.hpp>
#include "global.h"

class Ray
{
private:
 
public:
	sf::Vector2f direction;
	float length;
	int wallHit;
	bool isHitVertical;
	bool isHit;
	Ray();
	void cast(const sf::Vector2f& aPos, const sf::Vector2f& aRayDir, const std::vector<std::vector<int>>& aGrid);
};

