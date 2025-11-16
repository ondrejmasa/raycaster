#include "Player.h"



const bool Player::checkMapCollision(const gbl::map::MapType& aMap, const sf::Vector2f& aPos, const std::vector<Sprite>& aSprites) const
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
				y = (aPos.y + size / gbl::map::cellSize);
				break;
			case 2:
				x = (aPos.x + size / gbl::map::cellSize);
				y = aPos.y ;
				break;
			case 3:
				x = (aPos.x +  size / gbl::map::cellSize);
				y = (aPos.y +  size / gbl::map::cellSize);
				break;
		}
		if (aMap[int(y)][int(x)] > 0)
			return true;
	}
	for (const Sprite& s : aSprites)
	{
		const sf::Vector2f spriteSize(0.25f, 0.25f);
		const sf::Vector2f playereSize(0.25f, 0.25f);
		if (aPos.x + playereSize.x > s.position.x - spriteSize.x and
			aPos.x - playereSize.x < s.position.x + spriteSize.x and
			aPos.y + playereSize.y > s.position.y - spriteSize.y and
			aPos.y - playereSize.y < s.position.y + spriteSize.y)
		{
			return true;
		}
	}
	return false;
}

void Player::updateInput(const float aDeltaTime, sf::Window* aWindow, const std::vector<Sprite>& aSprites)
{
	sf::Vector2i center(gbl::screen::width / 2, gbl::screen::height / 2);
	sf::Vector2i mousePos = sf::Mouse::getPosition(*aWindow);
	int dx = mousePos.x - center.x;
	int dy = mousePos.y - center.y;

	// rotation
	float rotSpeed = 0.f;
	float oldDirX = direction.x;
	float oldPlaneX = plane.x;
	rotSpeed = dx * mouseSensitivity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
		rotSpeed = aDeltaTime * 3.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
		rotSpeed = -aDeltaTime * 3.f;
	direction.x = direction.x * cos(rotSpeed) - direction.y * sin(rotSpeed);
	direction.y = oldDirX * sin(rotSpeed) + direction.y * cos(rotSpeed);
	plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
	plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);

	// vertical movement
	pitch -= 1.3f*dy;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) pitch -= aDeltaTime * 200.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) pitch += aDeltaTime * 200.f;
	pitch = std::min(std::max(pitch, -gbl::screen::height / 2.f ), gbl::screen::height / 2.f );

	// position
	sf::Vector2f p;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) p += direction;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) p -= direction;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
	{
		p.x += direction.y;
		p.y += -direction.x;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
	{
		p.x += -direction.y;
		p.y += direction.x;
	}
	if (p.length() > 0)
	{
		p = p.normalized() * speed * aDeltaTime;

		sf::Vector2f newPos = position + p;
		if (!checkMapCollision(gbl::map::worldMap, newPos, aSprites))
			position = newPos;
		else if (!checkMapCollision(gbl::map::worldMap, sf::Vector2f(newPos.x, position.y), aSprites))
			position.x = newPos.x;
		else if (!checkMapCollision(gbl::map::worldMap, sf::Vector2f(position.x, newPos.y), aSprites))
			position.y = newPos.y;
	}

	sf::Mouse::setPosition(center, *aWindow);
}

Player::Player()
	: position(8.5f, 9.5f),
	  direction(1.f, 0.f),
	  plane(0.f, 0.66f)
{
}
