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
    std::vector<sf::Vector2f> path = {
        {480, 330}, {420, 330}, {420, 270}, {520, 270}, {520, 370}, {380, 370},
        {380, 230}, {560, 230}, {560, 410}, {340, 410}, {340, 190}, {600, 190},
        {600, 450}, {300, 450}, {300, 150}, {640, 150}, {640, 490}, {260, 490},
        {260, 110}, {680, 110}, {680, 530}, {220, 530}, {220, 70}, {720, 70},
        {720, 570}, {180, 570}, {180, 30}, {760, 30}
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
                sf::Vector2f pos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                towerController.addTower(pos);
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
