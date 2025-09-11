#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

class Game
{
private:
	sf::RenderWindow* window;
	Player player;
	sf::Clock clock;
	float deltaTime;
	int frameCount = 0;
	float fps = 0.0f;
	float timer = 0.f;

	void initWindow();
	void pollEvents();
	void updateFPS();
public:
	Game();
	virtual ~Game();

	void run();
	void update();
	void render();
};

