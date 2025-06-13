#include "view/scoreview.hpp"
#include "model/scoreloader.hpp"

#include <SFML/Graphics.hpp>



void drawScore(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/police/arial.ttf");

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("../assets/pictures/background_menu.PNG");
    
    sf::Sprite background(backgroundTexture);  

    auto scores = loadScores("../assets/data/scores.json");

    sf::Text score("Score :", font, 40);
    score.setPosition(450, 80);

    window.draw(background);   
    window.draw(score);   

    float y = 150.f;
    for (const auto& entry : scores) {
        sf::Text line(entry.name + ": " + std::to_string(entry.score), font, 30);
        line.setPosition(450.f, y);
        line.setFillColor(sf::Color::White);
        window.draw(line);
        y += 50.f;
    }
}
