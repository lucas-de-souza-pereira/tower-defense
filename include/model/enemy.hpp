#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path);

    void update(float dt); // avance sur le chemin
    sf::Vector2f getPosition() const;

private:
    std::vector<sf::Vector2f> path;
    int currentPoint;
    sf::Vector2f position;
    float speed;
};
