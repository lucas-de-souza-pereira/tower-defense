#include <SFML/Graphics.hpp>
#include "controller/enemycontroller.hpp"
#include "view/enemyview.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense - Debug View");

    // Définition du chemin (path) par des points
    std::vector<sf::Vector2f> path = {
        {100, 100},{300, 100},{300,175},{500, 175},{500,100},{700, 100}, {700, 500}, {100, 500}, {100, 800}
    };

    EnemyController controller(path);
    EnemyView view(window);

    sf::Clock clock;
    float spawnTimer = 0.f;

    while(window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        spawnTimer += deltaTime;
        if(spawnTimer > 1.0f) {
            controller.spawnEnemy();
            spawnTimer = 0.f;
        }

        controller.updateEnemies(deltaTime);

        window.clear(sf::Color::White);
        view.drawPath(controller.getPath());
        view.draw(controller.getEnemyPositions());
        window.display();
    }

    return 0;
}
