#include "view/enemyview.hpp"

EnemyView::EnemyView(sf::RenderWindow& win) : window(win) {}

void EnemyView::draw(const std::vector<sf::Vector2f>& enemyPositions) {
    for(const auto& pos : enemyPositions) {
        sf::RectangleShape square;
        square.setSize(sf::Vector2f(20, 20)); 
        square.setOrigin(10, 10); 
        square.setFillColor(sf::Color::Red);
        square.setPosition(pos);
        window.draw(square);
    }
}

void EnemyView::drawPath(const std::vector<sf::Vector2f>& pathPoints) {
    // Dessine les waypoints comme des petits cercles bleus
    for(const auto& point : pathPoints) {
        sf::CircleShape dot(5);
        dot.setFillColor(sf::Color::Blue);
        dot.setPosition(point.x - 5, point.y - 5);
        window.draw(dot);
    }
    // Dessine les segments entre les waypoints
    for(size_t i = 1; i < pathPoints.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(pathPoints[i-1], sf::Color::Blue),
            sf::Vertex(pathPoints[i], sf::Color::Blue)
        };
        window.draw(line, 2, sf::Lines);
    }
}
