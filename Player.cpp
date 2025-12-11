#include "Player.h"


void Player::updateInput(const float aDeltaTime, const sf::Vector2i& aMouseDelta, const Level& aLevel)
{
	// rotation
	float rotSpeed = 0.f;
	float oldDirX = direction.x;
	float oldPlaneX = plane.x;
	rotSpeed = aMouseDelta.x * mouseSensitivity;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
		rotSpeed = aDeltaTime * 3.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
		rotSpeed = -aDeltaTime * 3.f;
	direction.x = direction.x * cos(rotSpeed) - direction.y * sin(rotSpeed);
	direction.y = oldDirX * sin(rotSpeed) + direction.y * cos(rotSpeed);
	plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
	plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);

	// vertical movement
	pitch -= 1.3f* aMouseDelta.y;
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
		if (!aLevel.checkCollision(newPos, size))
			position = newPos;
		else if (!aLevel.checkCollision(sf::Vector2f(newPos.x, position.y), size))
			position.x = newPos.x;
		else if (!aLevel.checkCollision(sf::Vector2f(position.x, newPos.y), size))
			position.y = newPos.y;
	}
}

Player::Player()
	: position(8.5f, 9.5f),
	  direction(1.f, 0.f),
	  plane(0.f, 0.66f)
{
}
