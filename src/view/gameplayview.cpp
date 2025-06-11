#include "view/gameplayview.hpp"
#include <SFML/Graphics.hpp>

void drawGameplay(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/police/arial.ttf");

    sf::Text text("gameplay", font, 40);
    text.setFillColor(sf::Color::White);
    text.setPosition(300, 200);

    window.draw(text);
}
