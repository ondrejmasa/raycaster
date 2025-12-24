#pragma once
#include <vector>
#include <set>
#include "Sprite.h"
#include "Enemy.h"

class Level
{
private:
	void initializeSpriteGrid();
public:
	std::vector<std::vector<int>> grid;
	std::vector<std::shared_ptr<Sprite>> sprites;
	std::vector<std::vector<std::set<Sprite*>>> spriteGrid;
	Level();
	const bool checkCollision(const sf::Vector2f& aPos, const float aSize) const;
	void updateSpritePositions(const sf::Vector2f& aPlayerPos, const float aDeltaTime);
};

