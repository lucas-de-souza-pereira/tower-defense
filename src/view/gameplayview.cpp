#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

#include "controller/towercontroller.hpp"
#include "model/enemy.hpp"
#include "view/gameplayview.hpp"
#include "view/towerview.hpp"
#include "view/enemyview.hpp"

void drawGameplay(sf::RenderWindow& window) {
    // sf::RenderWindow window(sf::VideoMode(1200, 800), "Tower Defense - Debug View");

std::vector<sf::Vector2f> path = {
    {260,  50},  {260, 100},
    {360, 100},  {360, 200},
    {260, 200},  {260, 300},
    {360, 300},  {360, 400},
    {260, 400},  {260, 500},
    {360, 500},  {360, 600},
    {260, 600},  {260, 700},
    {360, 700},  {360, 800},
    {260, 800}
};


    TowerController towerController;
    TowerView towerView(window);
    EnemyView enemyView(window);

    std::vector<Enemy> enemies;

    sf::Clock deltaClock;
    sf::Clock gameClock;
    float spawnTimer = 0.f;

    int waveNumber = 1;
    int score = 0;
    int totalEnemiesSpawned = 0;

    int playerHP = 3;

    sf::Font font;
    font.loadFromFile("../assets/police/arial.ttf");

    sf::Text timerText("", font, 24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(20.f, 20.f);

    sf::Text waveText("", font, 24);
    waveText.setFillColor(sf::Color::Black);
    waveText.setPosition(20.f, 50.f);

    sf::Text scoreText("", font, 24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(20.f, 80.f);

    while (window.isOpen()) {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Tenter d'ajouter une tour à la position cliquée
                towerController.addTower(worldPos);
            }
        }

        spawnTimer += deltaTime;
        if (spawnTimer > 1.0f) {
            enemies.emplace_back(path);
            totalEnemiesSpawned++;
            spawnTimer = 0.f;

            if (totalEnemiesSpawned % 10 == 0) {
                waveNumber++;
            }
        }

        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }

        towerController.update(deltaTime, enemies);

        int before = enemies.size();
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.isAlive(); }),
            enemies.end()
        );
        int killed = before - enemies.size();
        score += killed * 10;

        
        int totalSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        timerText.setString("Time: " + std::to_string(minutes) + ":" +
                            (seconds < 10 ? "0" : "") + std::to_string(seconds));

        
        waveText.setString("Wave: " + std::to_string(waveNumber));
        scoreText.setString("Score: " + std::to_string(score));

       
        window.clear(sf::Color::White);
        enemyView.drawPath(path);
        enemyView.draw(enemies);
        towerView.draw(towerController.getTowers());

        window.draw(timerText);
        window.draw(waveText);
        window.draw(scoreText);
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
