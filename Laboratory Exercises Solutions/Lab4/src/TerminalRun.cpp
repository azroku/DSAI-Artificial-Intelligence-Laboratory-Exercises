#include "PuzzleState.h"
#include "Heuristics.h"
#include "AStarSearch.h" 
#include <iostream>

int TerminaRun() {
    // Define the start and goal states
    const PuzzleState startState({{1, 2, 3}, {0, 4, 6}, {7, 5, 8}}); 
    const PuzzleState goalState({{1, 2, 3}, {4, 5, 6}, {7, 8, 0}});

    std::cout << "Start State:" << std::endl;
    startState.print();
    std::cout << "Goal State:" << std::endl;
    goalState.print();

    std::cout << "\nRunning A* search with Manhattan Distance heuristic..." << std::endl;

    // Run the search. We pass manhattanDistance as the heuristic function.
    std::vector<PuzzleState> path = aStarSearch(startState, goalState, manhattanDistance);

    if (path.empty()) {
        std::cout << "Failed to find a solution." << std::endl;
    } else {
        std::cout << "\nSolution found! Path length: " << path.size() - 1 << " moves." << std::endl;
        std::cout << "Sequence of states:" << std::endl;
        for (const auto& state : path) {
            state.print();
        }
    }

    return 0;
}