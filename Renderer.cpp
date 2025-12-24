#include "Renderer.h"

void Renderer::renderWorld(sf::RenderTarget* aWindow, const std::vector<Ray>& aRays, const Player& aPlayer, std::vector<std::shared_ptr<Sprite>>& aSprites)
{
	// floor and ceil
	float horizon = gbl::screen::height / 2.f + aPlayer.pitch;
	//sky
	float angle = std::atan2(aPlayer.direction.y, aPlayer.direction.x);
	if (angle < 0.f)
		angle += static_cast<float>(2 * gbl::PI);
	float a = static_cast<float>(angle / (2 * gbl::PI));
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
	sf::Vector2f rayDirLeft = aPlayer.direction - aPlayer.plane;
	sf::Vector2f rayDirRight = aPlayer.direction + aPlayer.plane;
	sf::Image& floorImg = Resources::getFloorImage();
	sf::Image& ceilImg = Resources::getCeilImage();
	std::unique_ptr<uint8_t[]> floorPixels = std::make_unique<uint8_t[]>(gbl::screen::width * gbl::screen::height * 4);
	const uint8_t* floorTexData = floorImg.getPixelsPtr();
	const uint8_t* ceilTexData = ceilImg.getPixelsPtr();
	unsigned int texWidth = floorImg.getSize().x;
	unsigned int texHeight = floorImg.getSize().y;
	//for (unsigned short y = 0; y < gbl::screen::height; ++y)
	for (unsigned short y = static_cast<unsigned short>(horizon)+1; y < gbl::screen::height; ++y)
	{
		bool isFloor = y > horizon;
		float p = isFloor ? y - horizon : horizon - y;
		float rowDist = posZ / p;
		if (rowDist > gbl::map::maxRayLength)
			continue;
		sf::Vector2f floorStep = rowDist * (rayDirRight - rayDirLeft) / static_cast<float>(gbl::screen::width);
		sf::Vector2f floorPos = aPlayer.position + rowDist * rayDirLeft;
		uint8_t* rowPtr = floorPixels.get() + y * gbl::screen::width * 4;
		for (unsigned short x = 0; x < gbl::screen::width; ++x)
		{
			sf::Vector2i cell(floorPos);
			unsigned  tx = unsigned((floorPos.x - cell.x) * texWidth) & (texWidth - 1);
			unsigned  ty = unsigned((floorPos.y - cell.y) * texHeight) & (texHeight - 1);

			float shade = 1 - rowDist / gbl::map::maxRayLength;
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
	for (auto& p : lines) {
		p.second.clear();
	}
	for (unsigned short x = 0; x < aRays.size(); ++x)
	{
		const Ray& r = aRays[x];
		if (!r.isHit) continue;
		float lineH = gbl::screen::height / r.length;
		float s = (gbl::screen::height - lineH) / 2 + aPlayer.pitch;
		float e = (gbl::screen::height + lineH) / 2 + aPlayer.pitch;
		auto& tex = Resources::getWallTexture(r.wallHit - 1);
		double wallX;
		if (r.isHitVertical)
			wallX = aPlayer.position.y + r.length * r.direction.y;
		else
			wallX = aPlayer.position.x + r.length * r.direction.x;
		wallX -= floor(wallX);

		int texX = static_cast<int>(wallX * tex.getSize().x);

		if ((r.isHitVertical and r.direction.x < 0) or (!r.isHitVertical && r.direction.y > 0))
		{
			texX = tex.getSize().x - texX - 1;
		}
		double step = 64.0 / lineH;
		double texPos = (s - (gbl::screen::height - lineH) / 2 - aPlayer.pitch);

		float brightness = 1.f - r.length / gbl::map::maxRayLength;
		if (r.isHitVertical) brightness *= 0.7f;
		std::uint8_t shade = static_cast<uint8_t>(255.f * brightness);
		sf::Color c(shade, shade, shade);
		sf::Vertex v1{ sf::Vector2f(static_cast<float>(x), s), c, sf::Vector2f(static_cast<float>(texX), 0.f) };
		sf::Vertex v2{ sf::Vector2f(static_cast<float>(x), e), c, sf::Vector2f(static_cast<float>(texX),static_cast<float>(tex.getSize().y)) };

		sf::VertexArray& va = lines[r.wallHit - 1];
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
		[aPlayer](const auto& sprite)
		{
			return std::pow(aPlayer.position.x - sprite->position.x, 2) +
				std::pow(aPlayer.position.y - sprite->position.y, 2);
		};

	std::sort(aSprites.begin(), aSprites.end(),
		[&](const auto& a, const auto& b)
		{
			double distA = caclSpriteDist(a);
			double distB = caclSpriteDist(b);
			return distA > distB;
		});

	for (size_t i = 0; i < aSprites.size(); ++i)
	{
		const Sprite& sprite = *aSprites[i].get();
		float spriteX = sprite.position.x - aPlayer.position.x;
		float spriteY = sprite.position.y - aPlayer.position.y;
		float invDet = 1.0f / (aPlayer.plane.x * aPlayer.direction.y - aPlayer.direction.x * aPlayer.plane.y);
		float transX = invDet * (aPlayer.direction.y * spriteX - aPlayer.direction.x * spriteY);
		float transY = invDet * (-aPlayer.plane.y * spriteX + aPlayer.plane.x * spriteY);
		if (transY < 0.f) continue;
		int spriteScreenX = int((gbl::screen::width / 2.f) * (1 + transX / transY));
		int spriteSize = abs(int(gbl::screen::height / (transY)));
		int spriteHeight = static_cast<int>(spriteSize * sprite.scale);
		int drawStartY = static_cast<int>(-spriteHeight + spriteSize / 2.f + gbl::screen::height / 2.f + aPlayer.pitch);
		int drawEndY = static_cast<int>(spriteSize / 2.f + gbl::screen::height / 2.f + aPlayer.pitch);
		int spriteWidth = static_cast<int>(spriteSize * sprite.scale);
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		sf::Texture& tex = Resources::getSpriteTexture(sprite.texture);
		spriteStripes.clear();
		for (int stripe = drawStartX; stripe < drawEndX; ++stripe)
		{
			if (transY > 0 && stripe > 0 && stripe < gbl::screen::width && (transY) < aRays[stripe].length)
			{
				int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * tex.getSize().x / spriteWidth) / 256;
				sf::Vector2i texStart(texX, 0);
				sf::Vector2i texEnd(texX, tex.getSize().y);
				sf::Vector2i vertStart(stripe, drawStartY);
				sf::Vector2i vertEnd(stripe, drawEndY);
				sf::Vertex v1{ static_cast<sf::Vector2f>(vertStart), sf::Color::White, static_cast<sf::Vector2f>(texStart) };
				sf::Vertex v2{ static_cast<sf::Vector2f>(vertEnd), sf::Color::White, static_cast<sf::Vector2f>(texEnd) };
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

void Renderer::renderMap(sf::RenderTarget* aWindow, const std::vector<Ray>& aRays, const Player& aPlayer, const std::vector<std::vector<int>>& aGrid, const float aScale)
{
	constexpr float a = std::min(static_cast<float>(gbl::screen::height), static_cast<float>(gbl::screen::width));
	const size_t r = aGrid.size();
	const size_t c = r != 0u ? aGrid[0].size() : 0u;
	const float cellSize = static_cast<float>(std::min(gbl::screen::height, gbl::screen::width)) / static_cast<float>(r);
	sf::RectangleShape rBase(aScale * sf::Vector2f(static_cast<float>(c * cellSize), static_cast<float>(r * cellSize)));
	aWindow->draw(rBase);
	sf::VertexArray cells(sf::PrimitiveType::Triangles);
	for (size_t i = 0; i < r; ++i)
	{
		for (size_t j = 0; j < c; ++j)
		{
			float x = aScale * (j * cellSize);
			float y = aScale * (i * cellSize);
			float s = aScale * cellSize;
			sf::Vector2f p0(x, y);
			sf::Vector2f p1(x + s, y);
			sf::Vector2f p2(x + s, y + s);
			sf::Vector2f p3(x, y + s);
			sf::Color c = (aGrid[i][j] > 0)
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
	sf::CircleShape p(aScale * (aPlayer.size / 2) * cellSize);
	p.setFillColor(sf::Color::Green);
	p.setPosition(aScale * aPlayer.position * cellSize);
	aWindow->draw(p);

	sf::VertexArray lines(sf::PrimitiveType::Lines);
	for (int x = 0; x < gbl::screen::width; x += 20)
	{
		const Ray& r = aRays[x];
		if (!r.isHit) continue;
		float l = r.length * aScale * cellSize;
		sf::Vertex v1{ aPlayer.position * aScale * cellSize, sf::Color::Blue };
		sf::Vertex v2{ aPlayer.position * aScale * cellSize + l * r.direction, sf::Color::Blue };
		lines.append(v1);
		lines.append(v2);
	}
	aWindow->draw(lines);
}

Renderer::Renderer()
	: floorTexture(sf::Vector2u(gbl::screen::width, gbl::screen::height)),
	  buffer(sf::Vector2u(gbl::screen::width, gbl::screen::height)),
	  spriteStripes(sf::PrimitiveType::Lines)
{

}
