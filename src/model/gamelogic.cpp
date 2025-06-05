#include "model/gamelogic.hpp"

#include <SFML/Graphics.hpp>

void runGame() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(200, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}
