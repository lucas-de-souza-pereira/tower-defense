#include "view/scoreview.hpp"
#include "model/scoreloader.hpp"

#include <SFML/Graphics.hpp>



void drawScore(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/police/arial.ttf");

    auto scores = loadScores("../assets/data/scores.json");

    float y = 150.f;
    for (const auto& entry : scores) {
        sf::Text line(entry.name + ": " + std::to_string(entry.score), font, 30);
        line.setPosition(300.f, y);
        line.setFillColor(sf::Color::White);
        window.draw(line);
        y += 50.f;
    }
}
