#include "view/gameview.hpp"
#include "view/menuview.hpp"
#include "view/gameplayview.hpp"
#include "view/scoreview.hpp"

#include <iostream>

GameView::GameView()
    : window(sf::VideoMode(800, 600), "Tower Defense"), state(ViewState::MENU) {
    window.setPosition({400, 50});
}

void GameView::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

#include "model/inputhandler.hpp"
#include "model/gamestate.hpp"

InputHandler inputHandler;

void GameView::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (state == ViewState::MENU && event.type == sf::Event::MouseButtonPressed) {
            GameState result = inputHandler.handleMenuClick({event.mouseButton.x, event.mouseButton.y}, window);


            // Convertir le GameState en ViewState
            switch (result) {
                case GameState::GAME:   state = ViewState::GAMEPLAY; break;
                case GameState::SCORE:  state = ViewState::SCORE; break;
                case GameState::QUIT:   window.close(); break;
                default: break;
            }
        }



    }
}


void GameView::render() {
    window.clear();
    switch (state) {
        case ViewState::MENU:
            renderMenu();
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
void GameView::renderGameplay()  { drawGameplay(window); }
void GameView::renderScore()     { drawScore(window); }
