#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Sprite.h"
#include "Pathfinding.h"
#include "Ray.h"

class Enemy : public Sprite
{
public:
	float speed;
	sf::Vector2f direction;
	Enemy(const sf::Vector2f& aPos, const int aTex, const float aScale, const float aSize = 0.25f, const float aSpeed = 2.f);
	void updateDirection(const std::vector<std::vector<int>>& aGrid, const sf::Vector2f& aPlayerPos, const float aDeltaTime);
};

