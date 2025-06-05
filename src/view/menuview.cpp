#include "view/menuview.hpp"
#include <SFML/Graphics.hpp>


void drawMenu(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/arial.ttf");

    sf::Text play("Play", font, 40);
    play.setPosition(300, 200);

    sf::Text score("Score", font, 40);
    score.setPosition(300, 300);

    window.draw(play);
    window.draw(score);
}
