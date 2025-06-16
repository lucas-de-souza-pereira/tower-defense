#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

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
    // Configuration initiale
    std::vector<sf::Vector2f> path = { 
    {100, 100},   
    {500, 100},   
    {500, 500},   
    {100, 500},   
    {100, 180},   
    {420, 180},   
    {420, 420},   
    {180, 420},   
    {180, 260},   
    {340, 260},   
    {340, 340},   
    {260, 340},   
    {260, 300},   
    {300, 300}  
    };

    // Système de grille
    Grid grid(path);
    TowerController towerController;
    TowerView towerView(window);
    EnemyView enemyView(window);

    // Éléments de jeu
    std::vector<Enemy> enemies;
    sf::Clock clock;
    int playerLives = 3;
    int currentWave = 1;
    const int maxWaves = 10;
    int enemiesPerWave = 3; 
    int enemiesSpawned = 0;
    int enemiesDefeated = 0;
    bool waveInProgress = true;
    float spawnInterval = 1.0f; 
    float spawnTimer = 0.f;
    int towersAvailable = 1;
    int money = 0;
    int enemiesKilledThisFrame = 0;
    int upgradeCost = 50;
    int selectedTowerIndex = -1;

    sf::Vector2f currentTowerPos;

    // UI
    sf::Font font;
    bool fontLoaded = font.loadFromFile("../assets/police/arial.ttf");
    if (!fontLoaded) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
        // On continue quand même, le texte sera affiché sans police personnalisée
    }
    
    sf::RectangleShape upgradeButton(sf::Vector2f(150, 40));
    upgradeButton.setFillColor(sf::Color(100, 200, 250));
    sf::Text upgradeButtonText;
    if (fontLoaded) {
        upgradeButtonText.setFont(font);
    }
    upgradeButtonText.setString("Améliorer dégâts (-50)");
    upgradeButtonText.setCharacterSize(18);
    upgradeButtonText.setFillColor(sf::Color::Black);

    sf::Text livesText;
    if (fontLoaded) {
        livesText.setFont(font);
    }
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Black);
    livesText.setPosition(10, 10);

    // Aperçu de placement
    sf::CircleShape placementPreview(20);
    placementPreview.setOutlineThickness(2);
    placementPreview.setOutlineColor(sf::Color::Black);

    sf::Text moneyText;
    if (fontLoaded) {
        moneyText.setFont(font);
    }
    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::Black);
    moneyText.setPosition(10, 70);

    while(window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        // Gestion des événements
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseMoved) {
                currentTowerPos = grid.snapToGrid(
                    window.mapPixelToCoords(sf::Mouse::getPosition(window))
                );
            }

            if(event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    {event.mouseButton.x, event.mouseButton.y}
                );

                // 1. Si clique sur bouton upgrade (s’il est visible)
                if (selectedTowerIndex != -1 &&
                    upgradeButton.getGlobalBounds().contains(mousePos)) {
                    if (money >= upgradeCost) {
                        money -= upgradeCost;
                        towerController.upgradeDamage(selectedTowerIndex); // méthode à créer
                        std::cout << "Tour améliorée !" << std::endl;
                    } else {
                        std::cout << "Pas assez d'argent !" << std::endl;
                    }
                } 
                // 2. Sinon, clique sur une tour pour la sélectionner
                else {
                    selectedTowerIndex = towerController.getTowerIndexAtPosition(mousePos);
                    if (selectedTowerIndex == -1) {
                        std::cout << "Aucune tour sélectionnée" << std::endl;
                    } else {
                        std::cout << "Tour sélectionnée : " << selectedTowerIndex << std::endl;
                    }
                }

                // 3. Si clic sur la grille pour poser une tour (ancien code)
                sf::Vector2f snappedPos = grid.snapToGrid(mousePos);
                if (grid.isCellValid(snappedPos) && towersAvailable > 0) {
                    towerController.addTower(snappedPos);
                    grid.occupyCell(snappedPos);
                    towersAvailable--;
                } else if (towersAvailable <= 0) {
                    std::cout << "Plus de tours à poser pour le moment !" << std::endl;
                }
            }
        }

        // Système de vagues
        if (waveInProgress) {
            spawnTimer += deltaTime;
            // Spawn des ennemis tant qu'on n'a pas atteint le nombre pour cette vague
            if (enemiesSpawned < enemiesPerWave && spawnTimer > spawnInterval) {
                enemies.emplace_back(path);
                enemiesSpawned++;
                spawnTimer = 0.f;
            }
        }

        // Quand tous les ennemis de la vague sont morts, passe à la suivante
        if (waveInProgress && enemiesSpawned == enemiesPerWave && enemies.empty()) {
            waveInProgress = false;
        }

        for(auto& enemy : enemies) {
            enemy.update(deltaTime);
            if(enemy.hasReachedEnd()) {
                playerLives--;
                enemy.markForRemoval();
            }
        }

        // Calculer l'argent gagné avant nettoyage
        int enemiesKilledThisFrame = 0;
        for(const auto& enemy : enemies) {
            if (!enemy.isAlive() && !enemy.shouldBeRemoved()) {
                enemiesKilledThisFrame++;
            }
        }
        // Ajout de l'argent pour chaque ennemi tué
        money += enemiesKilledThisFrame * 10;

        towerController.update(deltaTime, enemies);

        // Nettoyage des ennemis
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [&](const Enemy& e) {
                    if (!e.isAlive() || e.shouldBeRemoved()) {
                        enemiesKilledThisFrame++;
                        return true;
                    }
                    return false;
                }),
            enemies.end()
        );

        money += enemiesKilledThisFrame * 10;

        if (selectedTowerIndex != -1) {
            sf::Vector2f towerPos = towerController.getTowers()[selectedTowerIndex].position;
            upgradeButton.setFillColor(sf::Color(255, 0, 0));
            upgradeButton.setOutlineThickness(2);
            upgradeButton.setOutlineColor(sf::Color::Black);

            upgradeButton.setPosition(towerPos.x, towerPos.y);
            upgradeButtonText.setPosition(towerPos.x, towerPos.y);

            window.draw(upgradeButton);
            window.draw(upgradeButtonText);
        }


        // Game Over
        if(playerLives <= 0) {
            // Création des textes et boutons
            sf::Text gameOverText;
            if (fontLoaded) gameOverText.setFont(font);
            gameOverText.setString("Game Over");
            gameOverText.setCharacterSize(60);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(350, 150);

            sf::RectangleShape retryButton(sf::Vector2f(200, 60));
            retryButton.setPosition(350, 300);
            retryButton.setFillColor(sf::Color(100, 200, 100));

            sf::Text retryText;
            if (fontLoaded) retryText.setFont(font);
            retryText.setString("Retry");
            retryText.setCharacterSize(30);
            retryText.setFillColor(sf::Color::Black);
            retryText.setPosition(390, 310);

            sf::RectangleShape menuButton(sf::Vector2f(200, 60));
            menuButton.setPosition(350, 400);
            menuButton.setFillColor(sf::Color(200, 100, 100));

            sf::Text menuText;
            if (fontLoaded) menuText.setFont(font);
            menuText.setString("Menu");
            menuText.setCharacterSize(30);
            menuText.setFillColor(sf::Color::Black);
            menuText.setPosition(420, 410);

            // Boucle d'attente de choix utilisateur
            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(
                            {event.mouseButton.x, event.mouseButton.y}
                        );
                        if (retryButton.getGlobalBounds().contains(mousePos)) {
                            return GameState::GAME; // Relance une partie
                        }
                        if (menuButton.getGlobalBounds().contains(mousePos)) {
                            return GameState::MENU; // Retourne au menu
                        }
                    }
                }

                window.clear(sf::Color::Black);
                window.draw(gameOverText);
                window.draw(retryButton);
                window.draw(retryText);
                window.draw(menuButton);
                window.draw(menuText);
                window.display();
            }
        }


        // Rendu
        window.clear(sf::Color::White);

        // Dessin de la grille
        sf::RectangleShape gridCell(sf::Vector2f(Grid::CELL_SIZE - 2, Grid::CELL_SIZE - 2));
        gridCell.setFillColor(sf::Color(200, 200, 200, 50));
        for(int x = 0; x < 1200; x += Grid::CELL_SIZE) {
            for(int y = 0; y < 800; y += Grid::CELL_SIZE) {
                gridCell.setPosition(x + 1, y + 1);
                window.draw(gridCell);
            }
        }

        // Aperçu de placement
        placementPreview.setPosition(currentTowerPos - sf::Vector2f(20, 20));
        placementPreview.setFillColor(grid.isCellValid(currentTowerPos) ? 
                                    sf::Color(0, 255, 0, 100) : 
                                    sf::Color(255, 0, 0, 100));
        window.draw(placementPreview);

        // Éléments de jeu
        enemyView.drawPath(path);
        enemyView.draw(enemies);
        towerView.draw(towerController.getTowers());

        // UI
        livesText.setString("Vies: " + std::to_string(playerLives));
        window.draw(livesText);
        // Affichage du numéro de vague
        sf::Text waveText;
        if (fontLoaded) waveText.setFont(font);
        waveText.setString("Vague: " + std::to_string(currentWave) + "/" + std::to_string(maxWaves));
        waveText.setCharacterSize(24);
        waveText.setFillColor(sf::Color::Black);
        waveText.setPosition(10, 40);
        window.draw(waveText);
        moneyText.setString("Argent: " + std::to_string(money));
        window.draw(moneyText);

        // Gestion du passage à la vague suivante
        if (!waveInProgress && currentWave < maxWaves) {
            sf::Text nextWaveText;
            if (fontLoaded) nextWaveText.setFont(font);
            nextWaveText.setString("Appuyez sur ESPACE\n pour la prochaine vague");
            nextWaveText.setCharacterSize(32);
            nextWaveText.setFillColor(sf::Color::Blue);
            nextWaveText.setPosition(300, 550);
            window.draw(nextWaveText);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                currentWave++;
                enemiesPerWave += 6;
                spawnInterval -= 0.1f; 
                enemiesSpawned = 0;
                waveInProgress = true;
                spawnTimer = 0.f;
                towersAvailable++;
                
            }
        }

        // Fin de la partie après la dernière vague
        if (!waveInProgress && currentWave == maxWaves && enemies.empty()) {
            sf::Text victoryText;
            if (fontLoaded) victoryText.setFont(font);
            victoryText.setString("Victoire !\nAppuyez sur ECHAP\n pour retourner au menu");
            victoryText.setCharacterSize(48);
            victoryText.setFillColor(sf::Color::Green);
            victoryText.setPosition(300, 500);
            window.draw(victoryText);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                return GameState::MENU;
            }
        }

        window.display();
    }

    return GameState::GAME; // Pour satisfaire le compilateur si la boucle se termine
}
