#pragma once
#include <vector>
#include <set>
#include "Sprite.h"
#include "Enemy.h"

class Level
{
private:
	void initializeSpriteGrid();
	const bool checkWallCollision(const sf::Vector2f& aPos, const float aSize) const;
	const bool checkSpriteCollision(const sf::Vector2f& aPos, const float aSize) const;
	void avoidSprites(const Enemy* aEnemy, sf::Vector2f& aPos) const;
public:
	std::vector<std::vector<int>> grid;
	std::vector<std::shared_ptr<Sprite>> sprites;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::vector<std::set<Sprite*>>> spriteGrid;
	Level();
	const bool checkCollision(const sf::Vector2f& aPos, const float aSize) const;
	void updateEnemyPositions(const sf::Vector2f& aPlayerPos, const float aDeltaTime);
};

