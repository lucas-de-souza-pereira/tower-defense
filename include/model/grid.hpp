#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Grid {
public:
    static const int CELL_SIZE = 40; // Taille des cellules
    static const int TOWER_RADIUS = 20; // Rayon des tours
    
    Grid(const std::vector<sf::Vector2f>& path);
    

    bool isCellValid(const sf::Vector2f& position) const;
    sf::Vector2f snapToGrid(const sf::Vector2f& position) const;
    void occupyCell(const sf::Vector2f& pos);
    bool isOnPath(const sf::Vector2f& pos) const;

private:
    std::vector<std::vector<bool>> pathCells;
    std::vector<sf::Vector2f> path;
    std::vector<std::vector<bool>> occupiedCells;
    bool isTooCloseToPath(const sf::Vector2f& pos) const;
};
