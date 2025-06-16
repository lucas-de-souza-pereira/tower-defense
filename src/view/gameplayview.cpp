#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>

#include "controller/towercontroller.hpp"
#include "model/enemy.hpp"
#include "model/grid.hpp"
#include "view/gameplayview.hpp"
#include "view/towerview.hpp"
#include "view/enemyview.hpp"
#include "controller/gamestate.hpp"
#include "controller/inputhandler.hpp"
#include "view/menuview.hpp"

GameState drawGameplay(sf::RenderWindow& window) {
    std::vector<sf::Vector2f> path = {
        {100, 100}, {500, 100}, {500, 500}, {100, 500}, {100, 180},
        {420, 180}, {420, 420}, {180, 420}, {180, 260}, {340, 260},
        {340, 340}, {260, 340}, {260, 300}, {300, 300}
    };

    Grid grid(path);
    TowerController towerController;
    TowerView towerView(window);
    EnemyView enemyView(window);

    std::vector<Enemy> enemies;
    sf::Clock clock;
    float spawnTimer = 0.f;
    int playerLives = 10;
    int playerMoney = 100;
    int gainParEnnemi = 10;
    int upgradeCost = 30;

    sf::Vector2f currentTowerPos;
    TowerController::Tower* selectedTower = nullptr;

    sf::Font font;
    bool fontLoaded = font.loadFromFile("../assets/police/arial.ttf");
    if (!fontLoaded) std::cerr << "Erreur de chargement de la police !" << std::endl;

    sf::Text livesText, moneyText;
    if (fontLoaded) {
        livesText.setFont(font);
        moneyText.setFont(font);
    }
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Black);
    livesText.setPosition(10, 10);

    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::Black);
    moneyText.setPosition(10, 40);

    sf::CircleShape placementPreview(20);
    placementPreview.setOutlineThickness(2);
    placementPreview.setOutlineColor(sf::Color::Black);

    float towerCooldownTimer = 30.f;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        towerCooldownTimer += deltaTime;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                currentTowerPos = grid.snapToGrid(
                    window.mapPixelToCoords(sf::Mouse::getPosition(window))
                );
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                bool clickedOnTower = false;

                for (auto& tower : towerController.towers) {
                    float dist = std::sqrt(std::pow(tower.position.x - mousePos.x, 2) + std::pow(tower.position.y - mousePos.y, 2));
                    if (dist < 20.f) {
                        selectedTower = &tower;
                        clickedOnTower = true;
                        break;
                    }
                }

                if (!clickedOnTower && towerCooldownTimer >= 30.f) {
                    sf::Vector2f snappedPos = grid.snapToGrid(mousePos);
                    if (grid.isCellValid(snappedPos)) {
                        towerController.addTower(snappedPos);
                        grid.occupyCell(snappedPos);
                        towerCooldownTimer = 0.f;
                    } else {
                        std::cout << "Placement interdit : cellule occupée ou sur le chemin." << std::endl;
                    }
                    selectedTower = nullptr;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (selectedTower && playerMoney >= upgradeCost) {
                    selectedTower->damage += 5;
                    playerMoney -= upgradeCost;
                }
            }
        }

        spawnTimer += deltaTime;
        if (spawnTimer > 1.0f) {
            enemies.emplace_back(path);
            spawnTimer = 0.f;
        }

        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
            if (enemy.hasReachedEnd()) {
                playerLives--;
                enemy.markForRemoval();
            }
        }

        towerController.update(deltaTime, enemies);

        for (auto& enemy : enemies) {
            if (!enemy.isAlive() && !enemy.shouldBeRemoved()) {
                playerMoney += gainParEnnemi;
                enemy.markForRemoval();
            }
        }

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.isAlive() || e.shouldBeRemoved(); }),
            enemies.end()
        );

        if (playerLives <= 0) {
            sf::Text gameOverText;
            if (fontLoaded) gameOverText.setFont(font);
            gameOverText.setString("Game Over!\nRetour au menu dans 3s...");
            gameOverText.setCharacterSize(60);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(300, 300);

            window.clear();
            window.draw(gameOverText);
            window.display();

            sf::Clock waitClock;
            while (waitClock.getElapsedTime().asSeconds() < 3) {}
            return GameState::MENU;
        }

        window.clear(sf::Color::White);

        sf::RectangleShape gridCell(sf::Vector2f(Grid::CELL_SIZE - 2, Grid::CELL_SIZE - 2));
        gridCell.setFillColor(sf::Color(200, 200, 200, 50));
        for (int x = 0; x < 1200; x += Grid::CELL_SIZE) {
            for (int y = 0; y < 800; y += Grid::CELL_SIZE) {
                gridCell.setPosition(x + 1, y + 1);
                window.draw(gridCell);
            }
        }

        placementPreview.setPosition(currentTowerPos - sf::Vector2f(20, 20));
        placementPreview.setFillColor(grid.isCellValid(currentTowerPos) ?
            sf::Color(0, 255, 0, 100) :
            sf::Color(255, 0, 0, 100));
        window.draw(placementPreview);

        enemyView.drawPath(path);
        enemyView.draw(enemies);
        towerView.draw(towerController.getTowers());

        livesText.setString("Vies: " + std::to_string(playerLives));
        moneyText.setString("Argent: " + std::to_string(playerMoney));
        window.draw(livesText);
        window.draw(moneyText);

        if (selectedTower) {
            sf::RectangleShape panel(sf::Vector2f(200, 100));
            panel.setFillColor(sf::Color(230, 230, 230));
            panel.setOutlineColor(sf::Color::Black);
            panel.setOutlineThickness(2);
            panel.setPosition(790, 10);
            window.draw(panel);

            sf::Text infoText;
            if (fontLoaded) infoText.setFont(font);
            infoText.setCharacterSize(18);
            infoText.setFillColor(sf::Color::Black);
            infoText.setString("Tour choisie\nDegats: " +
                std::to_string(selectedTower->damage) +
                "\nClic droit: +5 degats\nCout: " +
                std::to_string(upgradeCost));
            infoText.setPosition(800, 20);
            window.draw(infoText);
        }

        window.display();
    }

    return GameState::GAME;
}
