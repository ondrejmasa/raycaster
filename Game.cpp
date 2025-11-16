#include "Game.h"


void Game::initWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(gbl::screen::width, gbl::screen::height)), "Game 3", sf::Style::Close | sf::Style::Titlebar);
	window->setMouseCursorVisible(false);
}

void Game::pollEvents()
{
	while (const std::optional event = window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window->close();

		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				window->close();
		}
	}
}

void Game::updateFPS()
{
	deltaTime = clock.restart().asSeconds();
	timer += deltaTime;
	frameCount++;

	if (timer >= 1.0f)
	{
		fps = frameCount / timer;
		frameCount = 0;
		timer = 0.f;
		window->setTitle(std::to_string(fps));
	}
}

void Game::updateRays()
{
	for (unsigned short i = 0; i < rays.size(); ++i)
	{
		sf::Vector2f pc = player.position * gbl::map::cellSize + sf::Vector2f(player.size / 2, player.size / 2);
		float cameraX = 2 * i / float(gbl::screen::width) - 1;
		sf::Vector2f rayDir(player.direction + player.plane * cameraX);
		rays[i].cast(pc, rayDir);
	}
}

void Game::run()
{
	while (window->isOpen())
	{
		update();
		render();
	}
}

void Game::update()
{
	updateFPS();
	pollEvents();
	player.updateInput(deltaTime, window, sprites);
	updateRays();
}

void Game::render()
{
	window->clear();

	renderer.renderWorld(window, rays, player, sprites);
	renderer.renderMap(window, rays, player, 0.2);

	window->display();
}

Game::Game()
	: rays{ gbl::screen::width, Ray{} },
	  sprites{
		{ sf::Vector2f(8.0, 11.0), 0, 0.8f},
		{ sf::Vector2f(10.0, 11.0), 0, 1.f},
	  }
{
	initWindow();
}

Game::~Game()
{
	delete window;
}
