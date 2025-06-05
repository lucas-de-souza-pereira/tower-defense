#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class EnemyView {
private:
    sf::RenderWindow& window;

public:
    EnemyView(sf::RenderWindow& win);
    void draw(const std::vector<sf::Vector2f>& enemyPositions);
    void drawPath(const std::vector<sf::Vector2f>& pathPoints);
};
