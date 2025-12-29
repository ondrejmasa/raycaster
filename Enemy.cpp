#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& aPos, const int aTex, const float aScale, const float aSize, const float aSpeed)
	:	Sprite(aPos, aTex, aScale, aSize),
		speed{ aSpeed }
{
}

void Enemy::updateDirection(const std::vector<std::vector<int>>& aGrid, const sf::Vector2f& aPlayerPos)
{
    sf::Vector2f dirToPlayer = (aPlayerPos - position);
    sf::Vector2f sideStep = sf::Vector2f(-dirToPlayer.y, dirToPlayer.x).normalized() * size;
    Ray ray;
    bool isDirect = true;
	// test if enemy can see player directly if yes, go straight to him, else pathfinding
    for (float a = -1; a <= 1; a += 2)
    {
        sf::Vector2f shoulder = position + a * sideStep;
		sf::Vector2f dirShoulderToPlayer = (aPlayerPos - shoulder).normalized();
        float playerDistSqr = (shoulder.x - aPlayerPos.x) * (shoulder.x - aPlayerPos.x) + (shoulder.y - aPlayerPos.y) * (shoulder.y - aPlayerPos.y);
        ray.cast(shoulder, dirShoulderToPlayer, aGrid);
        if (ray.length * ray.length < playerDistSqr)
        {
            isDirect = false;
            break;
		}
	}
    if (isDirect)
    {
        if (dirToPlayer.lengthSquared() < maxDistToPlayer*maxDistToPlayer)
        {
            direction = sf::Vector2f(0.f, 0.f);
            return;
		}
        direction = dirToPlayer.normalized();
        
		// avoid walls
        for (short a = -1; a <= 1; a += 2)
        {
            sf::Vector2f sideDir = sf::Vector2f(-dirToPlayer.y, dirToPlayer.x) * static_cast<float>(a);
            Ray sideRay;
            sideRay.cast(position, sideDir, aGrid);
            if (sideRay.length < 0.5f)
            {
                direction += -sideDir * (0.5f - sideRay.length);
            }
        }
        direction = direction.normalized();
    }
	else
    {
		std::vector<sf::Vector2i> path = pf::aStarSearch(aGrid, sf::Vector2i(position), sf::Vector2i(aPlayerPos));
		size_t size = path.size();
		if (size <= 1) return;
		direction = sf::Vector2f(path[1].x + 0.5f, path[1].y + 0.5f) - position;
		direction = direction.normalized();
	}
}
