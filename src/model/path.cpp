#include "model/path.hpp"

Waypoint::Waypoint(float x, float y) : position(x, y), next(nullptr) {}

Path::Path() = default;

Path::~Path() {
    for(auto* wp : waypoints)
        delete wp;
}

void Path::addWaypoint(float x, float y) {
    auto* newWp = new Waypoint(x, y);
    if(!waypoints.empty())
        waypoints.back()->next = newWp;
    waypoints.push_back(newWp);
}

Waypoint* Path::getStart() const {
    return waypoints.empty() ? nullptr : waypoints.front();
}
