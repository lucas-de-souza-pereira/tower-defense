#pragma once
#include <vector>
#include <SFML/System.hpp>

class Waypoint {
public:
    sf::Vector2f position;
    Waypoint* next;
    
    Waypoint(float x, float y);
};

class Path {
private:
    std::vector<Waypoint*> waypoints;
    
public:
    Path();
    ~Path();
    
    void addWaypoint(float x, float y);
    Waypoint* getStart() const;
};
