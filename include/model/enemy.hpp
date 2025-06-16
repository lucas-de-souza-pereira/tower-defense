#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, int health = 10, float speed = 100.f);
    void update(float dt);
    sf::Vector2f getPosition() const;
    int getHealth() const;
    bool isAlive() const;
    bool hasReachedEnd() const;
    void markForRemoval();
    bool shouldBeRemoved() const;
    void takeDamage(int dmg);
private:
    bool markedForRemoval = false;
    bool reachedEnd = false;
    std::vector<sf::Vector2f> path;
    int currentPoint;
    sf::Vector2f position;
    float speed;
    int health;
    bool alive;
};
