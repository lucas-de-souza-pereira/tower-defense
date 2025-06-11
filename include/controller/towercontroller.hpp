#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "model/enemy.hpp"
#include "model/tower.hpp"
#include "model/grid.hpp"
#include <SFML/Graphics.hpp>
class TowerController {
private:
    

public:
    struct Tower {
        sf::Vector2f position;
        float range = 150.f;
        float fireRate = 1.f;
        float cooldown = 0.f;
        int damage = 10;
        bool selected = false;
        Tower(const sf::Vector2f& pos) : position(pos), range(150.f), fireRate(1.f), cooldown(0.f), damage(10), selected(false) {} 
    };

    std::vector<Tower> towers;
    void addTower(const sf::Vector2f& pos);
    void update(float deltaTime, std::vector<Enemy>& enemies);
    const std::vector<Tower>& getTowers() const;
};
