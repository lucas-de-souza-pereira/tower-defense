#pragma once
#include <SFML/Graphics.hpp>
#include "model/gamestate.hpp"

class InputHandler {
public:
    GameState handleMenuClick(const sf::Vector2i& mousePos, const sf::RenderWindow& window);
};
