#pragma once
#include <SFML/Graphics.hpp>
#include "global.h"

class Ray
{
private:
	sf::Vector2f positon;
	sf::Vector2f direction;
	float m_length;
	int m_wallHit;
	bool m_isHitVertical;
	bool m_isHit; 
public:
	Ray();
	const sf::Vector2f& getPositon() const;
	const sf::Vector2f& getDirection() const;
	const float& getLength() const;
	const int& getWallHit() const;
	const bool& getIsHitVertical() const;
	const bool& getIsHit() const;
	void cast(const sf::Vector2f& aPos, const sf::Vector2f& aRayDir);
};

