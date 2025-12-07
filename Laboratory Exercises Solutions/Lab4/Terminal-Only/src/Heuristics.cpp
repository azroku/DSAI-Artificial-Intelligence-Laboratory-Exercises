#include "Heuristics.h"
#include <cmath>
#include <vector>

// Heuristic 1: Counts the number of tiles that are not in their goal position.
int misplacedTiles(const PuzzleState& current, const PuzzleState& goal)
{
    int count = 0;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            // Ignore the blank tile
            if (current.board[r][c] == 0) {
                continue;
            }
            // If the tile in the current board is not the same as in the goal board, increment count
            if (current.board[r][c] != goal.board[r][c])
            {
                count++;
            }
        }
    }
    return count;
}

// Heuristic 2: Calculates the sum of Manhattan distances for all tiles.
int manhattanDistance(const PuzzleState& current, const PuzzleState& goal)
{
    int totalDistance = 0;

    // For efficiency, let's first map the goal positions of each tile (1-8)
    // This avoids searching for the goal position in a nested loop.
    std::vector<std::pair<int, int>> goalPositions(9);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            goalPositions[goal.board[r][c]] = {r, c};
        }
    }

    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int tileValue = current.board[r][c];

            // Ignore the blank tile
            if (tileValue == 0) {
                continue;
            }

            // Get the current position and the goal position of this tile
            std::pair<int, int> currentPos = {r, c};
            std::pair<int, int> goalPos = goalPositions[tileValue];

            // Calculate the Manhattan distance for this tile and add to the total
            totalDistance += std::abs(currentPos.first - goalPos.first) +
                             std::abs(currentPos.second - goalPos.second);
        }
    }
    return totalDistance;
}