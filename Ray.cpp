#include "Ray.h"

Ray::Ray()
{
}

void Ray::cast(const sf::Vector2f& aPos, const sf::Vector2f& aRayDir, const std::vector<std::vector<int>>& aGrid)
{
	direction = aRayDir;
	sf::Vector2i mapPos{ static_cast<sf::Vector2i>(aPos) };
	sf::Vector2f sideDist;
	sf::Vector2f deltaDist = sf::Vector2f(abs(1.f / aRayDir.x), abs(1.f / aRayDir.y));
	sf::Vector2i step;
	isHit = false;

	if (aRayDir.x < 0)
	{
		step.x = -1;
		sideDist.x = (aPos.x - mapPos.x) * deltaDist.x;
	}
	else
	{
		step.x = 1;
		sideDist.x = ((mapPos.x + 1) - aPos.x) * deltaDist.x;
	}
	if (aRayDir.y < 0)
	{
		step.y = -1;
		sideDist.y = (aPos.y - mapPos.y) * deltaDist.y;
	}
	else
	{
		step.y = 1;
		sideDist.y = ((mapPos.y + 1) - aPos.y) * deltaDist.y;
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
		if (aGrid[mapPos.y][mapPos.x] > 0)
		{
			isHit = true;
			wallHit = aGrid[mapPos.y][mapPos.x];
		}
	}

	length = currLen;
;}
