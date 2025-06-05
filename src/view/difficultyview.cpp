#include "view/difficultyview.hpp"
#include <SFML/Graphics.hpp>

void drawDifficulty(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/arial.ttf");

    sf::Text text("difficulty", font, 40);
    text.setFillColor(sf::Color::White);
    text.setPosition(300, 200);

    window.draw(text);
}
