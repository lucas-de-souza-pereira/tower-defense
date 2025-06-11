#pragma once
#include <SFML/Graphics.hpp>
#include "controller/towercontroller.hpp"

class TowerView {
private:
    sf::RenderWindow& window;

public:
    explicit TowerView(sf::RenderWindow& win);
    void draw(const std::vector<TowerController::Tower>& towers);
};
