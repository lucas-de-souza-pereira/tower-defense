#include "controller/towercontroller.hpp"
#include <cmath>

void TowerController::addTower(const sf::Vector2f& pos) {
    // Snapper la position à la grille
    sf::Vector2f snappedPos = Grid::snapToGrid(pos);

    int cellX = static_cast<int>(snappedPos.x) / Grid::CELL_SIZE;
    int cellY = static_cast<int>(snappedPos.y) / Grid::CELL_SIZE;

    // ❌ Si la case est interdite (ex: une sur deux)
    if ((cellX + cellY) % 2 != 0) {
        return; // position non autorisée (damier alterné)
    }

    // Vérifier si une tour est déjà à cette position
    for (const auto& tower : towers) {
        if (tower.position == snappedPos) {
            // Tour déjà présente, ne rien faire
            return;
        }
    }

    towers.push_back(Tower{snappedPos});
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
