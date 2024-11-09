#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include "raymath.h"

const int MAP_WIDTH = 8;
const int MAP_HEIGHT = 8;

struct Tile {
    Vector2 position;
    int gCost; // g = startista taman hetkiseen tileen
    int hCost; // h = g + goalin cost
    int fCost() const { return gCost + hCost; }
    Tile* parent;

    bool operator>(const Tile& other) const { return fCost() > other.fCost(); }
};

int heuristic(const Vector2& start, const Vector2& end);

bool isWalkable(const Vector2& pos, const std::vector<std::vector<int>>& MAP, Vector2 playerPos);

std::vector<Vector2> aStarSearch(const Vector2& start, const std::vector<std::vector<int>>& MAP, Vector2 playerPos);

#endif
