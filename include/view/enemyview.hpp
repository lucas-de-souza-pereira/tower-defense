#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "model/enemy.hpp"


class EnemyView {
    sf::RenderWindow& window;
public:
    EnemyView(sf::RenderWindow& win);
    void draw(const std::vector<Enemy>& enemies);
    void drawPath(const std::vector<sf::Vector2f>& pathPoints);
};

