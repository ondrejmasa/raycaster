#include "Player.h"



const bool Player::checkMapCollision(const gbl::map::MapType& aMap, const sf::Vector2f& aPos) const
{
	float x; float y;
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
		if (aMap[int(y)][int(x)] > 0)
			return true;
	}
	for (const Sprite& s : sprites)
	{
		const sf::Vector2f spriteSize(0.25f, 0.25f);
		const sf::Vector2f playereSize(0.25f, 0.25f);
		const sf::Vector2f playerPos = aPos / gbl::map::cellSize;
		if (playerPos.x + playereSize.x > s.position.x - spriteSize.x and
			playerPos.x - playereSize.x < s.position.x + spriteSize.x and
			playerPos.y + playereSize.y > s.position.y - spriteSize.y and
			playerPos.y - playereSize.y < s.position.y + spriteSize.y)
		{
			return true;
		}
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
		//for (const Sprite& s : sprites)
		//{
		//	sf::Vector2f diff = newPos / gbl::map::cellSize - s.position;
		//	float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		//	float minDist = 0.65f; // polomìr kolize

		//	if (dist < minDist)
		//	{
		//		float overlap = minDist - dist;
		//		// normalizovaný vektor od spritu k hráèi
		//		diff /= dist;
		//		newPos += diff * overlap;  // posun hráèe ven
		//		std::cout << overlap << '\n';
		//	}
		//}

		if (!checkMapCollision(gbl::map::worldMap, newPos))
			position = newPos;
		else if (!checkMapCollision(gbl::map::worldMap, sf::Vector2f(newPos.x, position.y)))
			position.x = newPos.x;
		else if (!checkMapCollision(gbl::map::worldMap, sf::Vector2f(position.x, newPos.y)))
			position.y = newPos.y;
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

	sf::VertexArray lines(sf::PrimitiveType::Lines);
	for (int x = 0; x < gbl::screen::width; x += 20)
	{
		Ray& r = rays[x];
		if (!r.getIsHit()) continue;
		float l = r.getLength() * scale;
		sf::Vertex v1{ r.getPositon() * scale, sf::Color::Blue };
		sf::Vertex v2{ r.getPositon() * scale + l * r.getDirection(), sf::Color::Blue };
		lines.append(v1);
		lines.append(v2);
	}
	aWindow->draw(lines);
}

void Player::renderWorld(sf::RenderTarget* aWindow)
{
	sf::RenderTexture buffer(sf::Vector2u(gbl::screen::width, gbl::screen::height));
	// floor and ceil
	float horizon = gbl::screen::height / 2.f + pitch;
		//sky
	float angle = std::atan2(direction.y, direction.x);
	if (angle < 0.f)
		angle += 2 * gbl::PI;
	float a = angle / (2 * gbl::PI);
	sf::Texture& skyTex = Resources::getSkyTexture();
	skyTex.setRepeated(true);
	constexpr float skyMoveSpeed = 5.f;
	float texOffX = skyTex.getSize().x * a * skyMoveSpeed;
	sf::VertexArray sky(sf::PrimitiveType::Triangles);
	sf::Vertex v1{ sf::Vector2f(0.f, horizon - (float)gbl::screen::height), sf::Color::White,
					sf::Vector2f(texOffX, 0.f) };
	sf::Vertex v2{ sf::Vector2f(0.f, horizon), sf::Color::White,
					sf::Vector2f(texOffX, (float)skyTex.getSize().y) };
	sf::Vertex v3{ sf::Vector2f((float)gbl::screen::width, horizon), sf::Color::White,
					sf::Vector2f(texOffX + (float)skyTex.getSize().x , (float)skyTex.getSize().y) };
	sf::Vertex v4{ sf::Vector2f((float)gbl::screen::width, horizon - (float)gbl::screen::height), sf::Color::White,
					sf::Vector2f(texOffX + (float)skyTex.getSize().x, 0.f) };
	sky.append(v1);
	sky.append(v2);
	sky.append(v3);
	sky.append(v1);
	sky.append(v3);
	sky.append(v4);
	//aWindow->draw(sky, &skyTex);
	buffer.draw(sky, &skyTex);
		// floor + ceil
	float posZ = 0.5f * gbl::screen::height;
	sf::Vector2f rayDirLeft = direction - plane;
	sf::Vector2f rayDirRight = direction + plane;
	sf::Image& floorImg = Resources::getFloorImage();
	sf::Image& ceilImg = Resources::getCeilImage();
	std::unique_ptr<uint8_t[]> floorPixels = std::make_unique<uint8_t[]>(gbl::screen::width * gbl::screen::height * 4);
	const uint8_t* floorTexData = floorImg.getPixelsPtr();
	const uint8_t* ceilTexData = ceilImg.getPixelsPtr();
	unsigned int texWidth = floorImg.getSize().x;
	unsigned int texHeight = floorImg.getSize().y;
	//for (unsigned short y = 0; y < gbl::screen::height; ++y)
	for (unsigned short y = horizon+1; y < gbl::screen::height; ++y)
	{
		bool isFloor = y > horizon;
		short p = isFloor ? y - horizon : horizon - y;
		float rowDist = posZ / p;
		if (rowDist * gbl::map::cellSize > gbl::map::maxRayLength)
			continue;
		sf::Vector2f floorStep = rowDist * (rayDirRight - rayDirLeft) / static_cast<float>(gbl::screen::width);
		sf::Vector2f floorPos = position / gbl::map::cellSize + rowDist * rayDirLeft;
		uint8_t* rowPtr = floorPixels.get() + y * gbl::screen::width * 4;
		for (unsigned short x = 0; x < gbl::screen::width; ++x)
		{
			sf::Vector2i cell(floorPos);
			unsigned  tx = unsigned((floorPos.x - cell.x) * texWidth) & (texWidth - 1);
			unsigned  ty = unsigned((floorPos.y - cell.y) * texHeight) & (texHeight - 1);

			float shade = 1 - rowDist * gbl::map::cellSize / gbl::map::maxRayLength;
			const uint8_t* TexData = isFloor ? floorTexData : ceilTexData;
			const uint8_t* floorTexPtr = TexData + (ty * texWidth + tx) * 4;
			rowPtr[0] = uint8_t(float(floorTexPtr[0]) * shade);
			rowPtr[1] = uint8_t(float(floorTexPtr[1]) * shade);
			rowPtr[2] = uint8_t(float(floorTexPtr[2]) * shade);
			rowPtr[3] = 255;
			rowPtr += 4;

			floorPos += floorStep;
		}
	}
	floorTexture.update(floorPixels.get(), sf::Vector2u(gbl::screen::width, gbl::screen::height), sf::Vector2u(0, 0));
	sf::Sprite fs{ floorTexture };
	buffer.draw(fs);

	// walls
	std::unordered_map<int, sf::VertexArray> lines;
	for (unsigned short x = 0; x < rays.size(); ++x)
	{
		Ray& r = rays[x];
		if (!r.getIsHit()) continue;
		float lineH = gbl::screen::height / r.getLength() * gbl::map::cellSize;
		float s = (gbl::screen::height - lineH) / 2 + pitch;
		float e = (gbl::screen::height + lineH) / 2 + pitch;
		auto& tex = Resources::getWallTexture(r.getWallHit() - 1);
		double wallX;
		if (r.getIsHitVertical())
			wallX = r.getPositon().y + r.getLength() * r.getDirection().y ;
		else
			wallX = r.getPositon().x + r.getLength() * r.getDirection().x;
		wallX -= floor(wallX / gbl::map::cellSize) * gbl::map::cellSize;

		int texX = wallX / gbl::map::cellSize * tex.getSize().x;

		if ((r.getIsHitVertical() and r.getDirection().x < 0) or (!r.getIsHitVertical() && r.getDirection().y > 0))
		{
			texX = tex.getSize().x - texX - 1;
		}
		double step = 64.0 / lineH;
		double texPos = (s - (gbl::screen::height - lineH) / 2 - pitch);

		float brightness = 1.f - r.getLength() / gbl::map::maxRayLength;
		if (r.getLength() / gbl::map::maxRayLength > 1.f)
		{
			std::cout << "bright: " << brightness << '\n';
			std::cout << "Length: " << r.getLength() << '\n';
		}
		if (r.getIsHitVertical()) brightness *= 0.7f;
		sf::Color c(255 * brightness, 255 * brightness, 255 * brightness);
		sf::Vertex v1{ sf::Vector2f(static_cast<float>(x), s), c, sf::Vector2f(texX, 0.f)};
		sf::Vertex v2{ sf::Vector2f(static_cast<float>(x), e), c, sf::Vector2f(texX, tex.getSize().y) };

		sf::VertexArray& va = lines[r.getWallHit() - 1];
		if (va.getPrimitiveType() == sf::PrimitiveType::Points) 
		{
			va.setPrimitiveType(sf::PrimitiveType::Lines); 
		}
		va.append(v1);
		va.append(v2);
	}
	for (auto& p : lines)
	{
		buffer.draw(p.second, &Resources::getWallTexture(p.first));
	}

	// Sprites
	auto caclSpriteDist =
		[this](const Sprite& sprite)
		{
			return std::pow(position.x / gbl::map::cellSize - sprite.position.x, 2) +
				std::pow(position.y / gbl::map::cellSize - sprite.position.y, 2);
		};

	std::sort(sprites.begin(), sprites.end(),
		[&](const Sprite& a, const Sprite& b)
		{
			double distA = caclSpriteDist(a);
			double distB = caclSpriteDist(b);
			return distA > distB;
		});

	for (size_t i = 0; i < sprites.size(); ++i)
	{
		Sprite& sprite = sprites[i];
		float spriteX = (sprite.position.x) - (position.x / gbl::map::cellSize);
		float spriteY = (sprite.position.y) - (position.y / gbl::map::cellSize);
		float invDet = 1.0f / (plane.x * direction.y - direction.x * plane.y);
		float transX = invDet * (direction.y * spriteX - direction.x * spriteY);
		double transY = invDet * (-plane.y * spriteX + plane.x * spriteY);
		if (transY < 0.f) continue;
		int spriteScreenX = int((gbl::screen::width / 2.f) * (1 + transX / transY));
		int spriteSize = abs(int(gbl::screen::height / (transY)));
		int spriteHeight = spriteSize * sprite.scale;
		int drawStartY = -spriteHeight + spriteSize/2.f + gbl::screen::height / 2.f + pitch;
		int drawEndY = spriteSize / 2.f + gbl::screen::height / 2.f + pitch;
		int spriteWidth = spriteSize * sprite.scale;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		sf::Texture& tex = Resources::getSpriteTexture(sprite.texture);
		sf::VertexArray spriteStripes(sf::PrimitiveType::Lines);
		for (int stripe = drawStartX; stripe < drawEndX; ++stripe)
		{
			if (transY > 0 && stripe > 0 && stripe < gbl::screen::width && (transY * gbl::map::cellSize) < rays[stripe].getLength())
			{
				int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * tex.getSize().x / spriteWidth) / 256;
				sf::Vector2f texStart = sf::Vector2f(texX, 0);
				sf::Vector2f texEnd = sf::Vector2f(texX, tex.getSize().y);
				sf::Vector2f vertStart(stripe, drawStartY);
				sf::Vector2f vertEnd(stripe, drawEndY);
				sf::Vertex v1{ vertStart, sf::Color::White, texStart };
				sf::Vertex v2{ vertEnd, sf::Color::White, texEnd };
				spriteStripes.append(v1);
				spriteStripes.append(v2);
			}
		}
		buffer.draw(spriteStripes, &tex);
	}


	// draw buffer to window
	buffer.display();
	sf::Sprite finS(buffer.getTexture());
	aWindow->draw(finS);
}

Player::Player()
	: position(250.f, 250.f),
	  direction(1.f, 0.f),
	  plane(0.f, 0.66f),
	  rays { gbl::screen::width, Ray{} },
	  floorTexture(sf::Vector2u(gbl::screen::width, gbl::screen::height)),
	  sprites{
		  { sf::Vector2f(8.0, 11.0), 0, 0.8f},
		  { sf::Vector2f(10.0, 11.0), 0, 1.f},
	  }
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
