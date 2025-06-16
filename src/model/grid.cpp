#include "model/grid.hpp"
#include <cmath>
#include <algorithm> // pour std::clamp

// Helper function to compute the dot product of two sf::Vector2f
static float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

// Helper function to compute the magnitude of a vector
static float magnitude(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Grid::Grid(const std::vector<sf::Vector2f>& path)
    : path(path)
{
    const int gridWidth = 1200 / CELL_SIZE;
    const int gridHeight = 800 / CELL_SIZE;
    occupiedCells.resize(gridWidth, std::vector<bool>(gridHeight, false));
    pathCells.resize(gridWidth, std::vector<bool>(gridHeight, false));

    // Marque toutes les cellules traversées par le chemin
    for (size_t i = 1; i < path.size(); ++i) {
        sf::Vector2f a = path[i-1];
        sf::Vector2f b = path[i];

        int x0 = static_cast<int>(a.x) / CELL_SIZE;
        int y0 = static_cast<int>(a.y) / CELL_SIZE;
        int x1 = static_cast<int>(b.x) / CELL_SIZE;
        int y1 = static_cast<int>(b.y) / CELL_SIZE;

        // Algorithme de tracé de segment sur grille (Bresenham)
        int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        while (true) {
            if (x0 >= 0 && y0 >= 0 && x0 < gridWidth && y0 < gridHeight)
                pathCells[x0][y0] = true;
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }
}

bool Grid::isCellValid(const sf::Vector2f& pos) const {
    int x = static_cast<int>(pos.x) / CELL_SIZE;
    int y = static_cast<int>(pos.y) / CELL_SIZE;
    if (x < 0 || y < 0 || x >= static_cast<int>(occupiedCells.size()) || y >= static_cast<int>(occupiedCells[0].size()))
        return false;
    // Interdit le placement si la cellule est occupée ou fait partie du chemin
    return !occupiedCells[x][y] && !isOnPath(pos);
}

sf::Vector2f Grid::snapToGrid(const sf::Vector2f& pos) const {
    return sf::Vector2f(
        std::floor(pos.x / CELL_SIZE) * CELL_SIZE + CELL_SIZE / 2,
        std::floor(pos.y / CELL_SIZE) * CELL_SIZE + CELL_SIZE / 2
    );
}

void Grid::occupyCell(const sf::Vector2f& pos) {
    int x = static_cast<int>(pos.x) / CELL_SIZE;
    int y = static_cast<int>(pos.y) / CELL_SIZE;
    if (x >= 0 && y >= 0 && x < static_cast<int>(occupiedCells.size()) && y < static_cast<int>(occupiedCells[0].size()))
        occupiedCells[x][y] = true;
}

bool Grid::isOnPath(const sf::Vector2f& pos) const {
    int x = static_cast<int>(pos.x) / CELL_SIZE;
    int y = static_cast<int>(pos.y) / CELL_SIZE;
    if (x >= 0 && y >= 0 && x < (int)pathCells.size() && y < (int)pathCells[0].size())
        return pathCells[x][y];
    return false;
}


bool Grid::isTooCloseToPath(const sf::Vector2f& pos) const {
    const float MIN_DISTANCE = CELL_SIZE * 1.5f;

    for (size_t i = 0; i + 1 < path.size(); i++) {
        sf::Vector2f a = path[i];
        sf::Vector2f b = path[i + 1];

        sf::Vector2f ab = b - a;
        sf::Vector2f ap = pos - a;

        float ab2 = dot(ab, ab);
        if (ab2 == 0) continue;

        float t = std::clamp(dot(ap, ab) / ab2, 0.f, 1.f);
        sf::Vector2f projection = a + t * ab;

        float distance = magnitude(pos - projection);
        if (distance < MIN_DISTANCE)
            return true;
    }
    return false;
}
