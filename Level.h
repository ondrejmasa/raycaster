#pragma once
#include <vector>
#include <set>
#include "Sprite.h"

class Level
{
private:
	void initializeSpriteGrid();
public:
	std::vector<std::vector<int>> grid;
	std::vector<Sprite> sprites;
	std::vector<std::vector<std::set<Sprite*>>> spriteGrid;
	const bool checkCollision(const sf::Vector2f& aPos, const float aSize) const;
	const std::vector<Sprite*> getSpritesAt(const sf::Vector2f& aPos) const;

	Level();
};

