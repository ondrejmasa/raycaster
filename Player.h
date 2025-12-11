#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "global.h"
#include "Level.h"

class Player
{
private:
	const float speed = 3.5f;
	const float mouseSensitivity = 0.003f;
public:
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Vector2f plane;
	float pitch = 0.f;
	const float size = 0.25f;
	Player();
	void updateInput(const float aDeltaTime, const sf::Vector2i& aMouseDelta, const Level& aLevel);
};

