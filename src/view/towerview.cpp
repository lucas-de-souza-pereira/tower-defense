#include "view/towerview.hpp"
#include "controller/towercontroller.hpp"
#include <SFML/Graphics.hpp>

TowerView::TowerView(sf::RenderWindow& win) : window(win) {}

void TowerView::draw(const std::vector<TowerController::Tower>& towers) {
    for(const auto& tower : towers) {
        sf::RectangleShape rect(sf::Vector2f(30, 30));
        rect.setFillColor(sf::Color::Green);
        rect.setOrigin(15, 15);
        rect.setPosition(tower.position);
        window.draw(rect);
    }
}