#include "Level.h"

void Level::initializeSpriteGrid()
{
    for (auto& row : spriteGrid) 
	{
        for (auto& cell : row) {
            cell.clear();
        }
	}

	auto addToGrid = [&](const auto& container)
	{
		for (const auto& sprite : container)
		{
			int x = static_cast<int>(sprite->position.x);
			int y = static_cast<int>(sprite->position.y);
			spriteGrid[y][x].insert(sprite.get());
		}
	};
	addToGrid(sprites);
	//addToGrid(enemies);
}

const bool Level::checkWallCollision(const sf::Vector2f& aPos, const float aSize) const
{
	if (aPos.x < 0.f or aPos.y < 0.f or aPos.x + aSize >= static_cast<float>(grid[0].size()) or aPos.y + aSize >= static_cast<float>(grid.size()))
		return true;
	float x; float y;
	for (unsigned char i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0:
			x = aPos.x;
			y = aPos.y;
			break;
		case 1:
			x = aPos.x;
			y = (aPos.y + aSize);
			break;
		case 2:
			x = (aPos.x + aSize);
			y = aPos.y;
			break;
		case 3:
			x = (aPos.x + aSize);
			y = (aPos.y + aSize);
			break;
		}
		if (grid[int(y)][int(x)] > 0)
			return true;
	}
	return false;
}

const bool Level::checkSpriteCollision(const sf::Vector2f& aPos, const float aSize) const
{
	sf::Vector2i p(static_cast<sf::Vector2i>(aPos));
	for (short i = -1; i <= 1; ++i)
	{
		for (short j = -1; j <= 1; ++j)
		{
			int cx = p.x + i;
			int cy = p.y + j;
			if (cy < 0 or cy >= static_cast<int>(spriteGrid.size()) or
				cx < 0 or cx >= static_cast<int>(spriteGrid[0].size()))
				continue;
			for (const Sprite* s : spriteGrid[cy][cx])
			{
				if (std::abs(aPos.x - s->position.x) < (aSize + s->size) and
					std::abs(aPos.y - s->position.y) < (aSize + s->size))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Level::avoidSprites(const Enemy* aEnemy, sf::Vector2f& aPos) const
{
	sf::Vector2i p(static_cast<sf::Vector2i>(aPos));
	for (short i = -1; i <= 1; ++i)
	{
		for (short j = -1; j <= 1; ++j)
		{
			int cx = p.x + i;
			int cy = p.y + j;
			if (cy < 0 or cy >= static_cast<int>(spriteGrid.size()) or
				cx < 0 or cx >= static_cast<int>(spriteGrid[0].size()))
				continue;
			for (const Sprite* s : spriteGrid[cy][cx])
			{
				if (s == aEnemy) continue;
				sf::Vector2f diff = aPos - s->position;
				float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
				float minDist = s->size + aEnemy->size; // polomìr kolize

				if (dist < minDist)
				{
					float overlap = minDist - dist;
					// normalizovaný vektor od spritu k hráèi
					diff /= dist;
					aPos += diff * overlap;  // posun hráèe ven
				}
			}
		}
	}
}

const bool Level::checkCollision(const sf::Vector2f& aPos, const float aSize) const
{
	return checkWallCollision(aPos, aSize) or checkSpriteCollision(aPos, aSize);
}

void Level::updateEnemyPositions(const sf::Vector2f& aPlayerPos, const float aDeltaTime)
{
	static int frameCount = 0;
	constexpr int maxUpdateInterval = 10;
	for (auto it = enemies.begin(); it < enemies.end(); it++)
	{
		Enemy* enemyPtr = it->get();
		// spread out updates over multiple frames
		int idx = (it - enemies.begin()) % maxUpdateInterval;
		if (frameCount == idx)
		{
			enemyPtr->updateDirection(grid, aPlayerPos);
		}
		// old position in grid for updating
		int xo = static_cast<int>(enemyPtr->position.x);
		int yo = static_cast<int>(enemyPtr->position.y);
		// check collisions and update position
		sf::Vector2f newPos = enemyPtr->position + enemyPtr->direction * enemyPtr->speed * aDeltaTime;
		avoidSprites(enemyPtr, newPos);
		if (!checkWallCollision(sf::Vector2f(newPos.x, enemyPtr->position.y), enemyPtr->size))
			enemyPtr->position.x = newPos.x;
		if (!checkWallCollision(sf::Vector2f(enemyPtr->position.x, newPos.y), enemyPtr->size))
			enemyPtr->position.y = newPos.y;
		// update grid if moved
		int xn = static_cast<int>(enemyPtr->position.x);
		int yn = static_cast<int>(enemyPtr->position.y);
		if ((xo != xn or yo != yn) and (yn >= 0 and yn < spriteGrid.size() and xn >= 0 and xn < spriteGrid[0].size()))
		{
			spriteGrid[yo][xo].erase(enemyPtr);
			spriteGrid[yn][xn].insert(enemyPtr);
		}
	}
	frameCount++;
	if (frameCount >= maxUpdateInterval)
		frameCount = 0;
}

Level::Level()
	: grid{
        std::vector{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        std::vector{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
        std::vector{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        std::vector{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	},
	sprites{
		std::make_shared<Sprite>(sf::Vector2f(8.0f, 11.0f), 0, 0.8f, 0.25f * 0.8f),
		std::make_shared<Sprite>(sf::Vector2f(10.0f, 11.0f), 0, 1.0f, 0.25f)
	},
	enemies{ 
		std::make_shared<Enemy>(sf::Vector2f(12.0f, 11.0f), 2, 1.0f, 0.25f),
		std::make_shared<Enemy>(sf::Vector2f(14.0f, 13.0f), 2, 1.0f, 0.25f),
		std::make_shared<Enemy>(sf::Vector2f(18.0f, 15.0f), 2, 1.0f, 0.25f)
	},
	spriteGrid{ grid.size(), std::vector<std::set<Sprite*>>(grid[0].size()) }
{
    initializeSpriteGrid();
}
