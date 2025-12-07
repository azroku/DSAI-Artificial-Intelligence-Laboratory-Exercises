#pragma once
#include "PuzzleState.h"

// Heuristic 1: Counts the number of tiles that are not in their goal position.
int misplacedTiles(const PuzzleState& current, const PuzzleState& goal);

// Heuristic 2: Calculates the sum of Manhattan distances for all tiles.
int manhattanDistance(const PuzzleState& current, const PuzzleState& goal);