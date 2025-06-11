#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct Grid {
    static const int CELL_SIZE;
    static std::vector<sf::Vector2f> generateGridPositions(int width, int height);
};
