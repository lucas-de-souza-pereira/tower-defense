#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class TowerController {
private:
    

public:
    struct Tower {
        sf::Vector2f position;
        float range = 150.f;
        float fireRate = 1.f;
        float cooldown = 0.f;
    };

    std::vector<Tower> towers;
    void addTower(const sf::Vector2f& pos);
    void update(float deltaTime);
    const std::vector<Tower>& getTowers() const;
};
