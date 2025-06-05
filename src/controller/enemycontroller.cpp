#include "controller/enemycontroller.hpp"
#include <cmath>
#include <SFML/Graphics.hpp>


EnemyController::EnemyController(const std::vector<sf::Vector2f>& path) 
    : pathPoints(path) {}

void EnemyController::spawnEnemy() {
    if(!pathPoints.empty()) {
        enemies.push_back({pathPoints[0], 0}); // Position initiale + index du premier waypoint
    }
}

void EnemyController::updateEnemies(float deltaTime) {
    for(auto& enemy : enemies) {
        if(enemy.currentWaypointIndex >= pathPoints.size() - 1) continue;
        const sf::Vector2f& target = pathPoints[enemy.currentWaypointIndex + 1];
        
        sf::Vector2f direction = target - enemy.position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if(distance > 2.0f) { // Marge de 2 pixels pour éviter les oscillations
            direction /= distance;
            enemy.position += direction * speed * deltaTime;
        } else {
            enemy.currentWaypointIndex++;
        }
    }
}

std::vector<sf::Vector2f> EnemyController::getEnemyPositions() const {
    std::vector<sf::Vector2f> positions;
    for(const auto& enemy : enemies) {
        positions.push_back(enemy.position);
    }
    return positions;
}

const std::vector<sf::Vector2f>& EnemyController::getPath() const {
    return pathPoints;
}
