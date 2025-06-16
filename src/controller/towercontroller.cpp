#include "controller/towercontroller.hpp"
#include <cmath>

void TowerController::addTower(const sf::Vector2f& pos) {
    towers.push_back(Tower{pos});
}

void TowerController::upgradeDamage(int index) {
    if (index < 0 || index >= (int)towers.size()) return;
    towers[index].damage += 1;
}

int TowerController::getTowerIndexAtPosition(sf::Vector2f pos) {
    for (int i = 0; i < (int)towers.size(); ++i) {
        sf::Vector2f towerPos = towers[i].position;
        // Considère une zone autour de la tour (ex: 40x40)
        sf::FloatRect rect(towerPos.x - 20, towerPos.y - 20, 40, 40);
        if (rect.contains(pos)) {
            return i;
        }
    }
    return -1; // aucune tour trouvée
}

void TowerController::update(float deltaTime, std::vector<Enemy>& enemies) {
    for (auto& tower : towers) {
        if (tower.cooldown > 0.f) {
            tower.cooldown -= deltaTime;
            continue;
        }
        Enemy* target = nullptr;
        float minDist = tower.range;
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) continue;
            sf::Vector2f diff = enemy.getPosition() - tower.position;
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (dist <= tower.range && dist < minDist) {
                minDist = dist;
                target = &enemy;
            }
        }
        if (target) {
            target->takeDamage(tower.damage);
            tower.cooldown = 1.f / tower.fireRate;
        }
    }
}


const std::vector<TowerController::Tower>& TowerController::getTowers() const {
    return towers;
}
