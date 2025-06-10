#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int CELL_SIZE = 40;
const int COLUMNS = WINDOW_WIDTH / CELL_SIZE;
const int ROWS = WINDOW_HEIGHT / CELL_SIZE;

const float TOWER_RANGE = 100.0f;
const float MONSTER_RADIUS = 15.0f;
const float TOWER_RADIUS = 10.0f;

struct Monster {
    sf::CircleShape shape;
    int health;
    bool alive = true;
};

struct Tower {
    sf::CircleShape shape;
    sf::CircleShape rangeCircle;
    sf::Clock attackTimer;
    bool isAttacking = false;
    int damage = 5;
    float range = TOWER_RANGE;
    bool selected = false;
};

sf::Vector2f getCellCenter(int mouseX, int mouseY) {
    int gridX = mouseX / CELL_SIZE;
    int gridY = mouseY / CELL_SIZE;
    return {
        gridX * CELL_SIZE + CELL_SIZE / 2.0f,
        gridY * CELL_SIZE + CELL_SIZE / 2.0f
    };
}

bool isInsideCircle(sf::Vector2f point, sf::CircleShape& circle) {
    sf::Vector2f center = circle.getPosition();
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    return std::sqrt(dx * dx + dy * dy) <= circle.getRadius();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense");
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Grille
    std::vector<sf::RectangleShape> grid;
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLUMNS; ++x) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor(sf::Color::Transparent);
            cell.setOutlineColor(sf::Color(100, 100, 100));
            cell.setOutlineThickness(1);
            grid.push_back(cell);
        }
    }

    // Tours
    std::vector<Tower> towers;

    // Monstres
    std::vector<Monster> monsters;

    // Monstre rouge
    Monster red;
    red.health = 100;
    red.shape.setRadius(MONSTER_RADIUS);
    red.shape.setFillColor(sf::Color::Red);
    red.shape.setOrigin(MONSTER_RADIUS, MONSTER_RADIUS);
    red.shape.setPosition(getCellCenter(std::rand() % WINDOW_WIDTH, std::rand() % WINDOW_HEIGHT));
    monsters.push_back(red);

    // Monstre violet
    Monster purple;
    purple.health = 1000;
    purple.shape.setRadius(MONSTER_RADIUS);
    purple.shape.setFillColor(sf::Color(128, 0, 128));
    purple.shape.setOrigin(MONSTER_RADIUS, MONSTER_RADIUS);
    purple.shape.setPosition(getCellCenter(std::rand() % WINDOW_WIDTH, std::rand() % WINDOW_HEIGHT));
    monsters.push_back(purple);

    // UI bouton "Ameliorer"
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Erreur : police manquante (arial.ttf)\n";
        return -1;
    }

    sf::RectangleShape upgradeDamageButton(sf::Vector2f(120, 30));
    sf::Text upgradeDamageText("Ameliorer Degats", font, 14);

    sf::RectangleShape upgradeRangeButton(sf::Vector2f(120, 30));
    sf::Text upgradeRangeText("Ameliorer Portee", font, 14);

    sf::Text statsText("", font, 14);
    statsText.setFillColor(sf::Color::White);

    bool showUpgradeUI = false;
    Tower* selectedTower = nullptr;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f clickPos = {(float)event.mouseButton.x, (float)event.mouseButton.y};
                sf::Vector2f center = getCellCenter(event.mouseButton.x, event.mouseButton.y);

                bool clickedOnTower = false;
                for (auto& tower : towers) {
                    if (isInsideCircle(clickPos, tower.shape)) {
                        tower.selected = true;
                        selectedTower = &tower;
                        showUpgradeUI = true;
                        clickedOnTower = true;
                    } else {
                        tower.selected = false;
                    }
                }

                // Clique sur bouton amelioration
                bool clickedUpgradeButton = false;
                if (showUpgradeUI && selectedTower != nullptr) {
                    if (upgradeDamageButton.getGlobalBounds().contains(clickPos)) {
                        selectedTower->damage += 5;
                        clickedUpgradeButton = true;
                    } else if (upgradeRangeButton.getGlobalBounds().contains(clickPos)) {
                        selectedTower->range += 15.0f;
                        selectedTower->rangeCircle.setRadius(selectedTower->range);
                        selectedTower->rangeCircle.setOrigin(selectedTower->range, selectedTower->range);
                        clickedUpgradeButton = true;
                    }
                }

                
                if (!clickedOnTower && !clickedUpgradeButton) {
                    // Placer une tour uniquement si la case est vide
                    bool occupied = false;
                    for (const auto& tower : towers) {
                        if (tower.shape.getPosition() == center) {
                            occupied = true;
                            break;
                        }
                    }

                    if (!occupied) {
                        Tower tower;
                        tower.shape.setRadius(TOWER_RADIUS);
                        tower.shape.setOrigin(TOWER_RADIUS, TOWER_RADIUS);
                        tower.shape.setPosition(center);
                        tower.shape.setFillColor(sf::Color::Blue);

                        tower.rangeCircle.setRadius(tower.range);
                        tower.rangeCircle.setOrigin(tower.range, tower.range);
                        tower.rangeCircle.setPosition(center);
                        tower.rangeCircle.setFillColor(sf::Color(0, 255, 0, 50));
                        tower.rangeCircle.setOutlineColor(sf::Color::Green);
                        tower.rangeCircle.setOutlineThickness(1);

                        towers.push_back(tower);
                        showUpgradeUI = false;
                        selectedTower = nullptr;
                    }
                }
            }
        }

        // Attaques des tours
        for (auto& tower : towers) {
            tower.isAttacking = false;
            for (auto& monster : monsters) {
                if (!monster.alive) continue;

                float dx = tower.shape.getPosition().x - monster.shape.getPosition().x;
                float dy = tower.shape.getPosition().y - monster.shape.getPosition().y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance <= tower.range) {
                    if (tower.attackTimer.getElapsedTime().asMilliseconds() >= 300) {
                        monster.health -= tower.damage;
                        tower.isAttacking = true;
                        tower.attackTimer.restart();
                        if (monster.health <= 0) {
                            monster.alive = false;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Grille
        for (const auto& cell : grid) {
            window.draw(cell);
        }

        // Tours
        for (auto& tower : towers) {
            window.draw(tower.rangeCircle);
            tower.shape.setFillColor(tower.isAttacking ? sf::Color::Yellow : (tower.selected ? sf::Color::Cyan : sf::Color::Blue));
            window.draw(tower.shape);
        }

        // Monstres
        for (const auto& monster : monsters) {
            if (monster.alive)
                window.draw(monster.shape);
        }

        // UI d'amelioration
        if (showUpgradeUI && selectedTower != nullptr) {
            sf::Vector2f uiPos = selectedTower->shape.getPosition() + sf::Vector2f(30, -20);

            // Affichage stats
            std::string statsStr = "Degats: " + std::to_string(selectedTower->damage) + 
                                "\nPortee: " + std::to_string((int)selectedTower->range);
            statsText.setString(statsStr);
            statsText.setPosition(uiPos.x, uiPos.y - 40);
            window.draw(statsText);

            // Bouton degats
            upgradeDamageButton.setPosition(uiPos);
            upgradeDamageButton.setFillColor(sf::Color(70, 70, 70, 200));
            upgradeDamageButton.setOutlineColor(sf::Color::White);
            upgradeDamageButton.setOutlineThickness(1);
            upgradeDamageText.setPosition(uiPos.x + 5, uiPos.y + 5);

            // Bouton portee
            sf::Vector2f rangeButtonPos = uiPos + sf::Vector2f(0, 40);
            upgradeRangeButton.setPosition(rangeButtonPos);
            upgradeRangeButton.setFillColor(sf::Color(70, 70, 70, 200));
            upgradeRangeButton.setOutlineColor(sf::Color::White);
            upgradeRangeButton.setOutlineThickness(1);
            upgradeRangeText.setPosition(rangeButtonPos.x + 5, rangeButtonPos.y + 5);

            // Dessin
            window.draw(upgradeDamageButton);
            window.draw(upgradeDamageText);
            window.draw(upgradeRangeButton);
            window.draw(upgradeRangeText);
        }

        window.display();
    }

    return 0;
}
