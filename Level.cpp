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
	addToGrid(enemies);
}

const bool Level::checkCollision(const sf::Vector2f& aPos, const float aSize) const
{
	if (aPos.x < 0.f or aPos.y < 0.f or
		aPos.x + aSize >= static_cast<float>(grid[0].size()) or
		aPos.y + aSize >= static_cast<float>(grid.size()))
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
					//return true;
				}
			}
		}
	}
	return false;
}

void Level::updateEnemyPositions(const sf::Vector2f& aPlayerPos, const float aDeltaTime)
{
	for (auto& enemy : enemies)
	{
		Enemy* enemyPtr = enemy.get();
		int xo = static_cast<int>(enemyPtr->position.x);
		int yo = static_cast<int>(enemyPtr->position.y);
		enemy->updateDirection(grid, aPlayerPos, aDeltaTime);
		sf::Vector2f newPos = enemy->position + enemy->direction * enemy->speed * aDeltaTime;
		if (!checkCollision(sf::Vector2f(newPos.x, enemy->position.y), enemy->size))
			enemy->position.x = newPos.x;
		if (!checkCollision(sf::Vector2f(enemy->position.x, newPos.y), enemy->size))
			enemy->position.y = newPos.y;
		int xn = static_cast<int>(enemyPtr->position.x);
		int yn = static_cast<int>(enemyPtr->position.y);
		if ((xo != xn or yo != yn) and (yn >= 0 and yn < spriteGrid.size() and xn >= 0 and xn < spriteGrid[0].size()))
		{
			spriteGrid[yo][xo].erase(enemyPtr);
			spriteGrid[yn][xn].insert(enemyPtr);
		}
	}
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
		std::make_shared<Enemy>(sf::Vector2f(12.0f, 11.0f), 2, 1.0f, 0.25f) 
	},
	spriteGrid{ grid.size(), std::vector<std::set<Sprite*>>(grid[0].size()) }
{
    initializeSpriteGrid();
}
