#pragma once
#include <SFML/Graphics.hpp>

enum class ViewState { MENU, GAMEPLAY, DIFFICULTY, SCORE };

class GameView {
public:

    static constexpr unsigned int WINDOW_WIDTH = 1000;
    static constexpr unsigned int WINDOW_HEIGHT = 900;


    GameView();
    void run();  

private:
    sf::RenderWindow window;
    ViewState state;

    void processEvents();
    void render();

    void renderMenu();
    void renderGameplay();
    void renderDifficulty();
    void renderScore();
};
