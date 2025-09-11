#include "Game.h"


void Game::initWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(gbl::screen::width, gbl::screen::height)), "Game 3", sf::Style::Close | sf::Style::Titlebar);
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
		std::cout << "FPS: " << fps << std::endl;
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
	player.update(deltaTime);
}

void Game::render()
{
	window->clear();

	player.render(window);

	window->display();
}

Game::Game()
{
	initWindow();
}

Game::~Game()
{
	delete window;
}
