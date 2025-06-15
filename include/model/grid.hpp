// grid.hpp
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct Grid {
    static inline const int CELL_SIZE = 32;

    // Génère toutes les positions centrales des cases dans une zone width x height (pixels)
    static std::vector<sf::Vector2f> generateGridPositions(int width, int height) {
        std::vector<sf::Vector2f> positions;
        for (int y = 0; y < height; y += CELL_SIZE) {
            for (int x = 0; x < width; x += CELL_SIZE) {
                positions.emplace_back(x + CELL_SIZE / 2.f, y + CELL_SIZE / 2.f);
            }
        }
        return positions;
    }

    // Trouve la position "snappée" à la grille la plus proche d'une position donnée
    static sf::Vector2f snapToGrid(const sf::Vector2f& pos) {
        int x = static_cast<int>(pos.x) / CELL_SIZE;
        int y = static_cast<int>(pos.y) / CELL_SIZE;
        return sf::Vector2f(x * CELL_SIZE + CELL_SIZE / 2.f, y * CELL_SIZE + CELL_SIZE / 2.f);
    }
};
