#include "model/enemy.hpp"
#include <cmath>

Enemy::Enemy(const std::vector<sf::Vector2f>& path, int health, float speed)
    : path(path), currentPoint(0), health(health), speed(speed), alive(true)
{
    if (!path.empty())
        position = path[0];
}

void Enemy::update(float dt) {
    if (!alive || currentPoint >= path.size() - 1) return;

    sf::Vector2f target = path[currentPoint + 1];
    sf::Vector2f dir = target - position;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist > 1.f) {
        dir /= dist;
        position += dir * speed * dt;
    } else {
        currentPoint++;
    }
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

int Enemy::getHealth() const {
    return health;
}

bool Enemy::isAlive() const {
    return alive;
}

void Enemy::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) alive = false;
}
