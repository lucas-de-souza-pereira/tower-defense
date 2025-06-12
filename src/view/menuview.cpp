#include "view/menuview.hpp"
#include <SFML/Graphics.hpp>

void drawMenu(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/police/arial.ttf");

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("../assets/pictures/background_menu.PNG");

    sf::Sprite background(backgroundTexture);  

    sf::Text play("Play", font, 40);
    play.setPosition(400, 200);

    sf::Text score("Score", font, 40);
    score.setPosition(400, 300);

    window.draw(background);   
    window.draw(play);
    window.draw(score);
}
