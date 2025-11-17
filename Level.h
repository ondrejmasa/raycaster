#pragma once
#include <vector>
#include "Sprite.h"

class Level
{
public:
	std::vector<std::vector<int>> grid;
	std::vector<Sprite> sprites;

	Level();
};

