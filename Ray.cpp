#include "Ray.h"

Ray::Ray()
{
}

void Ray::cast(const sf::Vector2f& aPos, const sf::Vector2f& aRayDir)
{
	positon = aPos;
	direction = aRayDir;
	sf::Vector2i mapPos{ static_cast<sf::Vector2i>(aPos / gbl::map::cellSize) };
	sf::Vector2f sideDist;
	sf::Vector2f deltaDist = sf::Vector2f(abs(1.f / aRayDir.x), abs(1.f / aRayDir.y)) * gbl::map::cellSize;
	sf::Vector2i step;
	isHit = false;

	if (aRayDir.x < 0)
	{
		step.x = -1;
		sideDist.x = (aPos.x - mapPos.x * gbl::map::cellSize) / gbl::map::cellSize * deltaDist.x;
	}
	else
	{
		step.x = 1;
		sideDist.x = ((mapPos.x + 1) * gbl::map::cellSize - aPos.x) / gbl::map::cellSize * deltaDist.x;
	}
	if (aRayDir.y < 0)
	{
		step.y = -1;
		sideDist.y = (aPos.y - mapPos.y * gbl::map::cellSize) / gbl::map::cellSize * deltaDist.y;
	}
	else
	{
		step.y = 1;
		sideDist.y = ((mapPos.y + 1) * gbl::map::cellSize - aPos.y) / gbl::map::cellSize * deltaDist.y;
	}

	float currLen = std::min(sideDist.x, sideDist.y);
	while (!isHit and currLen < gbl::map::maxRayLength)
	{
		if (sideDist.x < sideDist.y)
		{
			currLen = sideDist.x;
			sideDist.x += deltaDist.x;
			mapPos.x += step.x;
			isHitVertical = true;
		}
		else
		{
			currLen = sideDist.y;
			sideDist.y += deltaDist.y;
			mapPos.y += step.y;
			isHitVertical = false;
		}
		if (currLen > gbl::map::maxRayLength) break;
		if (gbl::map::worldMap[mapPos.y][mapPos.x] > 0)
		{
			isHit = true;
			wallHit = gbl::map::worldMap[mapPos.y][mapPos.x];
		}
	}

	length = currLen;
;}
