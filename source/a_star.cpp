#include "a_star.h"
#include <queue>
#include <cmath>
#include <algorithm>

int heuristic(const Vector2& start, const Vector2& end) 
{
    return abs(end.x - start.x) + abs(end.y - start.y);
}

bool isWalkable(const Vector2& pos, const std::vector<std::vector<int>>& MAP, Vector2 playerPos) 
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= MAP_WIDTH || pos.y >= MAP_HEIGHT) 
        return false;
    if (pos.x == playerPos.x && pos.y == playerPos.y)
        return false;
    return MAP[pos.y][pos.x] != 1;
}

std::vector<Vector2> aStarSearch(const Vector2& start, const std::vector<std::vector<int>>& MAP, Vector2 playerPos) 
{
    std::priority_queue<Tile, std::vector<Tile>, std::greater<Tile>> openSet;
    std::vector<std::vector<bool>> closedSet(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));
    std::vector<Vector2> treasures;

    // Etsitaan kaikki aarteet kartalla
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (MAP[y][x] == 2) treasures.push_back({ (float)x, (float)y });
        }
    }

    if (treasures.empty()) return {};

    // hyodynnetaan heuristiccia lahimman aarteen valintaan
    Vector2 closestTreasure = treasures[0];
    int closestDistance = heuristic(start, treasures[0]);
    for (const Vector2& treasure : treasures) {
        int dist = heuristic(start, treasure);
        if (dist < closestDistance) {
            closestDistance = dist;
            closestTreasure = treasure;
        }
    }

    Tile startNode = { start, 0, heuristic(start, closestTreasure), nullptr };
    openSet.push(startNode);

    Vector2 directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    while (!openSet.empty()) {
        Tile current = openSet.top();
        openSet.pop();

        closedSet[(int)current.position.y][(int)current.position.x] = true;

        if (current.position.x == closestTreasure.x && current.position.y == closestTreasure.y) {
            std::vector<Vector2> path;
            for (Tile* node = &current; node != nullptr; node = node->parent)
                path.push_back(node->position);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const Vector2& dir : directions) {
            Vector2 neighborPos = Vector2Add(current.position, dir);

            if (!isWalkable(neighborPos, MAP, playerPos) || closedSet[(int)neighborPos.y][(int)neighborPos.x]) continue;

            int gCost = current.gCost + 1;
            int hCost = heuristic(neighborPos, closestTreasure);
            Tile neighbor = { neighborPos, gCost, hCost, new Tile(current) };

            openSet.push(neighbor);
        }
    }

    return {};
}