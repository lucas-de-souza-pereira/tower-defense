#include <SFML/Graphics.hpp>
#include <vector>

#include "controller/towercontroller.hpp"

#include "model/enemy.hpp"

#include "view/gameplayview.hpp"
#include "view/towerview.hpp"
#include "view/enemyview.hpp"


void drawGameplay(sf::RenderWindow& window) {
    // sf::RenderWindow window(sf::VideoMode(1200, 800), "Tower Defense - Debug View");

    std::vector<sf::Vector2f> path = { {430, 330}, {370, 330}, {370, 270}, {470, 270}, {470, 370}, {330, 370},
        {330, 230}, {510, 230}, {510, 410}, {290, 410}, {290, 190}, {550, 190},
        {550, 450}, {250, 450}, {250, 150}, {590, 150}, {590, 490}, {210, 490},
        {210, 110}, {630, 110}, {630, 530}, {170, 530}, {170, 70}, {670, 70},
        {670, 570}, {130, 570}, {130, 30}, {710, 30}};
    TowerController towerController;
    TowerView towerView(window);
    EnemyView enemyView(window);

    std::vector<Enemy> enemies;
    sf::Clock clock;
    float spawnTimer = 0.f;

    while(window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Tenter d'ajouter une tour à la position cliquée
                towerController.addTower(worldPos);
            }
        }

        spawnTimer += deltaTime;
        if(spawnTimer > 1.0f) {
            enemies.emplace_back(path); // Constructeur Enemy(path)
            spawnTimer = 0.f;
        }

        for(auto& enemy : enemies) {
            enemy.update(deltaTime);
        }

        towerController.update(deltaTime, enemies);

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                           [](const Enemy& e){ return !e.isAlive(); }),
            enemies.end()
        );

        window.clear(sf::Color::White);
        enemyView.drawPath(path);
        enemyView.draw(enemies);
        towerView.draw(towerController.getTowers());
        window.display();
    }
}

void drawGrid(sf::RenderWindow& window, int width, int height) {
    sf::RectangleShape line;
    line.setFillColor(sf::Color(100, 100, 100, 100)); // gris transparent

    // lignes verticales
    for (int x = 0; x <= width; x += Grid::CELL_SIZE) {
        line.setSize(sf::Vector2f(1.f, static_cast<float>(height)));
        line.setPosition(static_cast<float>(x), 0.f);
        window.draw(line);
    }

    // lignes horizontales
    for (int y = 0; y <= height; y += Grid::CELL_SIZE) {
        line.setSize(sf::Vector2f(static_cast<float>(width), 1.f));
        line.setPosition(0.f, static_cast<float>(y));
        window.draw(line);
    }
}
