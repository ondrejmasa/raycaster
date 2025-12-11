#include "Level.h"

void Level::initializeSpriteGrid()
{
    for (auto& row : spriteGrid) {
        for (auto& cell : row) {
            cell.clear();
        }
	}
    for (Sprite& sprite : sprites)
    {
		Sprite* spritePtr = &sprite;
		int x = static_cast<int>(spritePtr->position.x);
		int y = static_cast<int>(spritePtr->position.y);
		spriteGrid[y][x].insert(spritePtr);
    }
}

const bool Level::checkCollision(const sf::Vector2f& aPos, const float aSize) const
{
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
					return true;
				}
			}
		}
	}
	return false;
}

const std::vector<Sprite*> Level::getSpritesAt(const sf::Vector2f& aPos) const
{
	std::vector<Sprite*> result;
	sf::Vector2i p(static_cast<sf::Vector2i>(aPos));
    for (short i = -1; i <= 1; ++i)
    {
        for (short j = -1; j <= 1; ++j)
        {
			if (p.y + j < 0 or p.y + j >= static_cast<int>(spriteGrid.size()) or
				p.x + i < 0 or p.x + i >= static_cast<int>(spriteGrid[0].size()))
				continue;
            for (Sprite* s : spriteGrid[p.y + j][p.x + i])
            {
                result.push_back(s);
			}
        }
	}
	return result;
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
      { sf::Vector2f(8.0, 11.0), 0, 0.8f, 0.25f*0.8f},
      { sf::Vector2f(10.0, 11.0), 0, 1.f, 0.25f},
    },
	spriteGrid{ grid.size(), std::vector<std::set<Sprite*>>(grid[0].size()) }
{
    initializeSpriteGrid();
}
