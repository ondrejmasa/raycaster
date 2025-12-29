#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "global.h"
#include "Resources.h"
#include "Ray.h"
#include "Sprite.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Level.h"


class Renderer
{
private:
	sf::Texture floorTexture;
	sf::RenderTexture buffer;
	std::unordered_map<int, sf::VertexArray> lines;
	sf::VertexArray spriteStripes;

public:
	void renderWorld(sf::RenderTarget* aWindow, const std::vector<Ray>& aRays, const Player& aPlayer, Level& aLevel);
	void renderMap(sf::RenderTarget* aWindow, const std::vector<Ray>& aRays, const Player& aPlayer, const std::vector<std::vector<int>>& aGrid, const float aScale);
	Renderer();
};

