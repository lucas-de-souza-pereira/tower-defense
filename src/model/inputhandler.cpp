#include "model/inputhandler.hpp"

GameState InputHandler::handleMenuClick(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    sf::FloatRect playBounds(300, 200, 200, 50);  
    sf::FloatRect scoreBounds(300, 300, 200, 50); 

    if (playBounds.contains(worldPos)) return GameState::GAME;
    if (scoreBounds.contains(worldPos)) return GameState::SCORE;

    return GameState::MENU;
}
