#include "../../include/controller/towercontroller.hpp"
#include <cmath>
#include "../../include/model/tower.hpp"
#include "../../include/model/enemy.hpp"
#include "../../include/model/grid.hpp"

#include "controller/towercontroller.hpp"

void TowerController::addTower(const sf::Vector2f& pos) {
    towers.emplace_back(Tower{pos});
}

void TowerController::update(float deltaTime) {
    for(auto& tower : towers) {
        if(tower.cooldown > 0.f)
            tower.cooldown -= deltaTime;
    }
}

const std::vector<TowerController::Tower>& TowerController::getTowers() const {
    return towers;
}
