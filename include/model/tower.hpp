#pragma once
#include <SFML/Graphics.hpp>

struct Tower {
    sf::Vector2f position;
    float range;
    float fireCooldown;
    float currentCooldown;
    int damage;
    bool selected;
};
