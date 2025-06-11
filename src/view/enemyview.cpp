#include "view/enemyview.hpp"
#include "model/enemy.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

EnemyView::EnemyView(sf::RenderWindow& win) : window(win) {}

void EnemyView::draw(const std::vector<Enemy>& enemies) {
    for(const auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        sf::RectangleShape rect(sf::Vector2f(20, 20));
        rect.setFillColor(sf::Color::Red);
        rect.setOrigin(10, 10);
        rect.setPosition(enemy.getPosition());
        window.draw(rect);
    }
}


void EnemyView::drawPath(const std::vector<sf::Vector2f>& pathPoints) {
    // Dessine les waypoints comme des petits cercles bleus
    for (const auto& point : pathPoints) {
        sf::CircleShape dot(5);
        dot.setFillColor(sf::Color::Blue);
        dot.setPosition(point.x - 5, point.y - 5);
        window.draw(dot);
    }
    // Dessine les segments entre les waypoints
    for (size_t i = 1; i < pathPoints.size(); ++i) {
        sf::Vertex line[] = {
            sf::Vertex(pathPoints[i-1], sf::Color::Blue),
            sf::Vertex(pathPoints[i], sf::Color::Blue)
        };
        window.draw(line, 2, sf::Lines);
    }
}
