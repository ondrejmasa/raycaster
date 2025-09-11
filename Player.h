#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "global.h"
#include "Ray.h"

class Player
{
private:
	const float speed = 100.f;
	sf::Vector2f position;
	sf::Vector2f direction;
	sf::Vector2f plane;
	std::array<Ray, gbl::screen::width> rays;
	bool checkMapCollision(const gbl::map::MapType& aMap, const sf::Vector2f& aPos);
	void updateInput(const float& aDeltaTime);
	void updateRays();
	void renderMap(sf::RenderTarget* aWindow);
	void renderWorld(sf::RenderTarget* aWindow);
public:
	const float size = 10.f;
	Player();
	void update(const float& aDeltaTime);
	void render(sf::RenderTarget* aWindow);
};

