#include "Enemy.h"

Enemy::Enemy(const sf::Vector2f& aPos, const int aTex, const float aScale, const float aSize, const float aSpeed)
	: Sprite(aPos, aTex, aScale, aSize)
{
	speed = aSpeed;
}

void Enemy::updatePos(const sf::Vector2f& aPlayerPos, const float aDeltaTime)
{
	sf::Vector2f direction = aPlayerPos - position;
	if (direction.length() > 1.f)
	{
		direction = direction.normalized();
		position += direction * speed * aDeltaTime;
	}
}
