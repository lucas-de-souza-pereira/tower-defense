#include "view/difficultyview.hpp"
#include "view/gameview.hpp"
#include "view/menuview.hpp"
#include "view/gameplayview.hpp"
#include "view/scoreview.hpp"


#include "controller/inputhandler.hpp"
#include "controller/gamestate.hpp"

#include <iostream>

GameView::GameView()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense"), state(ViewState::MENU) {
    window.setPosition({50, 50});
}

void GameView::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}


InputHandler inputHandler;

void GameView::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            GameState result = GameState::MENU;

            switch (state) {
                case ViewState::MENU:
                    result = inputHandler.handleMenuClick(mousePos, window);
                    break;
                case ViewState::DIFFICULTY:
                    result = inputHandler.handleDifficultyClick(mousePos, window);
                    break;
                case ViewState::GAMEPLAY:
                    result = inputHandler.handleGameplayClick(mousePos, window);
                    break;
                case ViewState::SCORE:
                    result = inputHandler.handleScoreClick(mousePos, window);
                    break;
            }

            // mise à jour de l’état courant
            switch (result) {
                case GameState::GAME:   state = ViewState::GAMEPLAY; break;
                case GameState::DIFFICULTY:   state = ViewState::DIFFICULTY; break;
                case GameState::SCORE:  state = ViewState::SCORE; break;
                case GameState::MENU:   state = ViewState::MENU; break;
                case GameState::QUIT:   window.close(); break;
            }
        }
    }
}

void GameView::renderGameplay() {
    GameState result = drawGameplay(window);
    if (result == GameState::MENU) {
        state = ViewState::MENU;
    }
}

void GameView::render() {
    window.clear();
    switch (state) {
        case ViewState::MENU:
            renderMenu();
            break;
        case ViewState::DIFFICULTY:
            renderDifficulty();
            break;
        case ViewState::GAMEPLAY:
            renderGameplay();
            break;
        case ViewState::SCORE:
            renderScore();
            break;
    }
    window.display();
}


void GameView::renderMenu()      { drawMenu(window); }
void GameView::renderDifficulty()     { drawDifficulty(window); }
void GameView::renderScore()     { drawScore(window); }
