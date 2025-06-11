#include <SFML/Graphics.hpp>
#include "controller/enemycontroller.hpp"
#include "view/enemyview.hpp"
#include "controller/towercontroller.hpp"
#include "view/towerview.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Tower Defense - Debug View");

    // Chemin des ennemis
    std::vector<sf::Vector2f> path = {
        {400, 300}, {410, 300}, {410, 310}, {390, 310}, {390, 290}, {430, 290},
        {430, 330}, {370, 330}, {370, 270}, {470, 270}, {470, 370}, {330, 370},
        {330, 230}, {510, 230}, {510, 410}, {290, 410}, {290, 190}, {550, 190},
        {550, 450}, {250, 450}, {250, 150}, {590, 150}, {590, 490}, {210, 490},
        {210, 110}, {630, 110}, {630, 530}, {170, 530}, {170, 70}, {670, 70},
        {670, 570}, {130, 570}, {130, 30}, {710, 30}
    };

    // Initialisation des contrôleurs
    EnemyController enemyController(path);
    TowerController towerController;

    // Initialisation des vues
    EnemyView enemyView(window);
    TowerView towerView(window);

    sf::Clock clock;
    float spawnTimer = 0.f;

    while(window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Gestion des événements
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                const sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                towerController.addTower(mousePos);
            }
        }

        // Spawn automatique d'ennemis
        spawnTimer += deltaTime;
        if(spawnTimer > 1.0f) {
            enemyController.spawnEnemy();
            spawnTimer = 0.f;
        }

        // Mise à jour de la logique
        enemyController.updateEnemies(deltaTime);
        towerController.update(deltaTime);

        // Rendu
        window.clear(sf::Color::White);
        enemyView.drawPath(enemyController.getPath());
        enemyView.draw(enemyController.getEnemyPositions());
        towerView.draw(towerController.getTowers());
        window.display();
    }

    return 0;
}
