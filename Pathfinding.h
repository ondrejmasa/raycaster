#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <set>

namespace pf
{
    typedef std::pair<int, int> Pair;

    std::vector<sf::Vector2i> aStarSearch(const std::vector<std::vector<int>>& grid, const sf::Vector2i& src, const sf::Vector2i& dest);
}