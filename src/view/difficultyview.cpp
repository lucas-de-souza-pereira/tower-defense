#include "view/difficultyview.hpp"
#include "view/gameview.hpp"

#include <SFML/Graphics.hpp>

void drawDifficulty(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("../assets/police/arial.ttf");

    sf::Text text_difficulty("Difficulty :", font, 40);
    text_difficulty.setFillColor(sf::Color::White);
    text_difficulty.setPosition(400, 80);

    sf::Text text_easy("Easy", font, 40);
    text_easy.setFillColor(sf::Color::White);
    text_easy.setPosition(400, 200);

    sf::Text text_normal("Normal", font, 40);
    text_normal.setFillColor(sf::Color::White);
    text_normal.setPosition(400, 300);

    sf::Text text_hard("Hard", font, 40);
    text_hard.setFillColor(sf::Color::White);
    text_hard.setPosition(400, 400);

    sf::Text text_return_menu("Return to menu : Right click", font, 20);
    text_return_menu.setFillColor(sf::Color::White);
    text_return_menu.setPosition(300, 600);

    window.draw(text_difficulty);
    window.draw(text_easy);
    window.draw(text_normal);
    window.draw(text_hard);
    window.draw(text_return_menu);
}
