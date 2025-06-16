#include "controller/inputhandler.hpp"

GameState InputHandler::handleMenuClick(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    if (sf::FloatRect(300, 200, 200, 50).contains(worldPos)) return GameState::DIFFICULTY;
    if (sf::FloatRect(300, 300, 200, 50).contains(worldPos)) return GameState::SCORE;
    return GameState::MENU;
}

GameState InputHandler::handleDifficultyClick(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    if (sf::FloatRect(400, 200, 200, 50).contains(worldPos)) return GameState::GAME;
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        return GameState::MENU;
    return GameState::GAME;
}

GameState InputHandler::handleGameplayClick(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        return GameState::MENU;
    return GameState::GAME;
}

GameState InputHandler::handleScoreClick(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        return GameState::MENU;
    return GameState::SCORE;
}
