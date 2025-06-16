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
    {100, 100},   // Départ en haut à gauche (coin extérieur)
    {500, 100},   // Coin supérieur droit
    {500, 500},   // Coin inférieur droit
    {100, 500},   // Coin inférieur gauche
    {100, 180},   // Deuxième boucle, coin haut-gauche
    {420, 180},   // Coin haut-droit
    {420, 420},   // Coin bas-droit
    {180, 420},   // Coin bas-gauche
    {180, 260},   // Troisième boucle, coin haut-gauche
    {340, 260},   // Coin haut-droit
    {340, 340},   // Coin bas-droit
    {260, 340},   // Coin bas-gauche
    {260, 300},   // Centre (optionnel)
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
    float spawnTimer = 0.f;
    int playerLives = 10;
    sf::Vector2f currentTowerPos;

    // UI
    sf::Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf");
    if (!fontLoaded) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
        // On continue quand même, le texte sera affiché sans police personnalisée
    }

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
                sf::Vector2f snappedPos = grid.snapToGrid(mousePos);

                if (grid.isCellValid(snappedPos)) {
                    towerController.addTower(snappedPos);
                    grid.occupyCell(snappedPos);  // Marque la cellule comme occupée
                }
                else {
                    std::cout << "Placement interdit : cellule occupée ou sur le chemin." << std::endl;
                }
            }
        }

        // Mise à jour du jeu
        spawnTimer += deltaTime;
        if(spawnTimer > 1.0f) {
            enemies.emplace_back(path);
            spawnTimer = 0.f;
        }

        for(auto& enemy : enemies) {
            enemy.update(deltaTime);
            if(enemy.hasReachedEnd()) {
                playerLives--;
                enemy.markForRemoval();
            }
        }

        towerController.update(deltaTime, enemies);

        // Nettoyage des ennemis
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e){ return !e.isAlive() || e.shouldBeRemoved(); }),
            enemies.end()
        );

        // Game Over
        if(playerLives <= 0) {
            sf::Text gameOverText;
            if (fontLoaded) {
                gameOverText.setFont(font);
            }
            gameOverText.setString("Game Over!\nRetour au menu dans 3s...");
            gameOverText.setCharacterSize(60);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(300, 300);
            
            window.clear();
            window.draw(gameOverText);
            window.display();
            
            sf::Clock waitClock;
            while(waitClock.getElapsedTime().asSeconds() < 3) {}
            return GameState::MENU; 
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

        window.display();
    }

    return GameState::GAME; // Pour satisfaire le compilateur si la boucle se termine
}
