#pragma once
#include <SFML/Graphics.hpp>
#include "Sprite.h"

class Enemy : public Sprite
{
private:
	float speed;
public:
	Enemy(const sf::Vector2f& aPos, const int aTex, const float aScale, const float aSize = 0.25f, const float aSpeed = 2.f);
	void updatePos(const sf::Vector2f& aPlayerPos, const float aDeltaTime) override;
};

