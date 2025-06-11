#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class EnemyController {
private:
    struct EnemyData {
        sf::Vector2f position;
        size_t currentWaypointIndex;
    };

    std::vector<EnemyData> enemies;
    std::vector<sf::Vector2f> pathPoints;
    float speed = 120.0f;

public:
    EnemyController(const std::vector<sf::Vector2f>& path);
    void spawnEnemy();
    void updateEnemies(float deltaTime);
    std::vector<sf::Vector2f> getEnemyPositions() const;
    const std::vector<sf::Vector2f>& getPath() const;
};
