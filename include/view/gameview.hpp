#pragma once
#include <SFML/Graphics.hpp>

enum class ViewState { MENU, GAMEPLAY, SCORE };

class GameView {
public:
    GameView();
    void run();  

private:
    sf::RenderWindow window;
    ViewState state;

    void processEvents();
    void render();

    void renderMenu();
    void renderGameplay();
    void renderScore();
};
