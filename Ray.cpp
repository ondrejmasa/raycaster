#include "Ray.h"

Ray::Ray()
{
}

const sf::Vector2f& Ray::getPositon() const
{
	return positon;
}

const sf::Vector2f& Ray::getDirection() const
{
	return direction;
}

const float& Ray::getLength() const
{
	return m_length;
}

const int& Ray::getWallHit() const
{
	return m_wallHit;
}

const bool& Ray::getIsHitVertical() const
{
	return m_isHitVertical;
}

const bool& Ray::getIsHit() const
{
	return m_isHit;
}

void Ray::cast(const sf::Vector2f& aPos, const sf::Vector2f& aRayDir)
{
	positon = aPos;
	direction = aRayDir;
	sf::Vector2i mapPos{ static_cast<sf::Vector2i>(aPos / gbl::map::cellSize) };
	sf::Vector2f sideDist;
	sf::Vector2f deltaDist = sf::Vector2f(abs(1.f / aRayDir.x), abs(1.f / aRayDir.y)) * gbl::map::cellSize;
	sf::Vector2i step;
	m_isHit = false;

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
	while (!m_isHit and currLen < gbl::map::maxRayLength)
	{
		if (sideDist.x < sideDist.y)
		{
			currLen = sideDist.x;
			sideDist.x += deltaDist.x;
			mapPos.x += step.x;
			m_isHitVertical = true;
		}
		else
		{
			currLen = sideDist.y;
			sideDist.y += deltaDist.y;
			mapPos.y += step.y;
			m_isHitVertical = false;
		}
		if (currLen > gbl::map::maxRayLength) break;
		if (gbl::map::worldMap[mapPos.y][mapPos.x] > 0)
		{
			m_isHit = true;
			m_wallHit = gbl::map::worldMap[mapPos.y][mapPos.x];
		}
	}

	m_length = currLen;
;}
