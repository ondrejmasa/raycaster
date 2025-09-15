#include "Player.h"



bool Player::checkMapCollision(const gbl::map::MapType& aMap, const sf::Vector2f& aPos)
{
	int x; int y;
	for (unsigned char i = 0; i < 4; ++i)
	{
		switch (i)
		{
			case 0:
				x = aPos.x  / gbl::map::cellSize;
				y = aPos.y / gbl::map::cellSize;
				break;
			case 1:
				x = aPos.x / gbl::map::cellSize;
				y = (aPos.y + size) / gbl::map::cellSize;
				break;
			case 2:
				x = (aPos.x + size) / gbl::map::cellSize;
				y = aPos.y / gbl::map::cellSize;
				break;
			case 3:
				x = (aPos.x +  size) / gbl::map::cellSize;
				y = (aPos.y +  size) / gbl::map::cellSize;
				break;
		}
		if (aMap[y][x] > 0)
			return true;
	}
	return false;
}

void Player::updateInput(const float& aDeltaTime, sf::Window* aWindow)
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
	pitch -= dy;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) pitch -= aDeltaTime * 200.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) pitch += aDeltaTime * 200.f;
	pitch = std::min(std::max(pitch, -gbl::screen::height / 1.f ), gbl::screen::height / 1.f );

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
		if (!checkMapCollision(gbl::map::worldMap, position + p))
			position = position + p;
		else if (!checkMapCollision(gbl::map::worldMap, sf::Vector2f(position.x + p.x, position.y)))
			position.x = position.x + p.x;
		else if (!checkMapCollision(gbl::map::worldMap, sf::Vector2f(position.x, position.y + p.y)))
			position.y = position.y + p.y;
	}

	sf::Mouse::setPosition(center, *aWindow);
}

void Player::updateRays()
{
	for (unsigned short i = 0; i < rays.size(); ++i)
	{
		sf::Vector2f pc = position + sf::Vector2f(size / 2, size / 2);
		float cameraX = 2 * i / float(gbl::screen::width) - 1;
		sf::Vector2f rayDir(direction + plane * cameraX);
		rays[i].cast(pc, rayDir);
	}
}

void Player::renderMap(sf::RenderTarget* aWindow)
{
	constexpr float scale = 0.2f;
	constexpr float a = std::min(static_cast<float>(gbl::screen::height), static_cast<float>(gbl::screen::width));
	sf::RectangleShape rBase(scale * sf::Vector2f(static_cast<float>(gbl::map::columns * gbl::map::cellSize), static_cast<float>(gbl::map::rows * gbl::map::cellSize)));
	aWindow->draw(rBase);
	sf::VertexArray cells(sf::PrimitiveType::Triangles);
	for (size_t i = 0; i < gbl::map::rows; ++i)
	{
		for (size_t j = 0; j < gbl::map::columns; ++j)
		{
			float x = scale * (j * gbl::map::cellSize);
			float y = scale * (i * gbl::map::cellSize);
			float s = scale * gbl::map::cellSize;
			sf::Vector2f p0(x, y);
			sf::Vector2f p1(x + s, y);
			sf::Vector2f p2(x + s, y + s);
			sf::Vector2f p3(x, y + s);
			sf::Color c = (gbl::map::worldMap[i][j] > 0)
				? sf::Color::Red
				: sf::Color::White;
			cells.append({ p0, c });
			cells.append({ p1, c });
			cells.append({ p2, c });
			cells.append({ p0, c });
			cells.append({ p2, c });
			cells.append({ p3, c });
		}
	}
	aWindow->draw(cells);
	sf::CircleShape c(scale * size / 2);
	c.setFillColor(sf::Color::Green);
	c.setPosition(scale * position);
	aWindow->draw(c);

	for (int x = 0; x < gbl::screen::width; x += 20)
	{
		float l = rays[x].getLength() * scale;
		sf::Vertex v1{ rays[x].getPositon() * scale, sf::Color::Blue };
		sf::Vertex v2{ rays[x].getPositon() * scale + l * rays[x].getDirection(), sf::Color::Blue };
		sf::Vertex line[] = { v1, v2 };
		aWindow->draw(line, 2, sf::PrimitiveType::Lines);
	}
}

void Player::renderWorld(sf::RenderTarget* aWindow)
{
	for (unsigned short x = 0; x < rays.size(); ++x)
	{
		Ray& r = rays[x];
		float lineH = gbl::screen::height / r.getLength() * gbl::map::cellSize;
		float s = (gbl::screen::height - lineH) / 2 + pitch;
		float e = (gbl::screen::height + lineH) / 2 + pitch;
		s = std::max(0.f, s);
		e = std::min(static_cast<float>(gbl::screen::height), e);
		sf::Color c = gbl::colors[r.getWallHit()];
		if (r.getIsHitVertical())
			c = sf::Color(c.r * 0.9f, c.g * 0.9f, c.b * 0.9f);
		sf::Vertex v1{ sf::Vector2f(static_cast<float>(x), s), c };
		sf::Vertex v2{ sf::Vector2f(static_cast<float>(x), e), c };
		sf::Vertex line[] = { v1, v2 };
		aWindow->draw(line, 2, sf::PrimitiveType::Lines);
	}
}

Player::Player()
	: position(250.f, 250.f),
	  direction(1.f, 0.f),
	  plane(0.f, 0.66f),
	  rays { }
{
}

void Player::update(const float& aDeltaTime, sf::Window* aWindow)
{
	updateInput(aDeltaTime, aWindow);
	updateRays();
}

void Player::render(sf::RenderTarget* aWindow)
{
	renderWorld(aWindow);
	renderMap(aWindow);
}
