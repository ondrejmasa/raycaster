#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Renderer.h"

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
	Renderer renderer;
	std::vector<Ray> rays;
	std::vector<Sprite> sprites;
	bool isGamePaused = false;
	const sf::Vector2i center = sf::Vector2i(gbl::screen::width / 2, gbl::screen::height / 2);
	Level level;

	void initWindow();
	void pollEvents();
	void updateFPS();
	void updateRays();
public:
	Game();
	virtual ~Game();

	void run();
	void update();
	void render();
};

