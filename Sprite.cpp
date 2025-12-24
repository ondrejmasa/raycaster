#include "Sprite.h"

Sprite::Sprite(const sf::Vector2f& aPos, const int aTex, const float aScale, const float aSize)
{
	position = aPos;
	texture = aTex;
	scale = aScale;
	size = aSize;
}
