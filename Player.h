#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cstddef>
#include "global.h"
#include "Ray.h"
#include "Resources.h"
#include "Sprite.h"

class Player
{
private:
	const float speed = 3.5f;
	const float mouseSensitivity = 0.003f;
	const bool checkMapCollision(const gbl::map::MapType& aMap, const sf::Vector2f& aPos, const std::vector<Sprite>& aSprites) const;
public:
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Vector2f plane;
	float pitch = 0.f;
	const float size = 10.f;
	Player();
	void updateInput(const float aDeltaTime, sf::Window* aWindow, const std::vector<Sprite>& aSprites);
};

