#include "PuzzleState.h"
#include <iostream>

// Constructor to initialize from a 3x3 vector
PuzzleState::PuzzleState(const std::vector<std::vector<int>>& initialBoard)
    : board(initialBoard)
{
}

// Find the position of the blank tile (0)
std::pair<int, int> PuzzleState::findBlank() const
{
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            if (board[r][c] == 0)
            {
                return {r, c};
            }
        }
    }
    // Return an invalid position if the blank is not found (error case)
    return {-1, -1};
}

// Overload operator< for use in std::set or as a key in std::map
bool PuzzleState::operator<(const PuzzleState& other) const
{
    // Leverage the default std::vector comparison, which performs
    // a lexicographical comparison. This is a simple and correct
    // way to establish a strict weak ordering for the states.
    return board < other.board;
}

// Overload operator== for easy comparison (e.g., checking against the goal state)
bool PuzzleState::operator==(const PuzzleState& other) const
{
    return board == other.board;
}

// Helper function to print the board to the console for debugging
void PuzzleState::print() const
{
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            std::cout << board[r][c] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---" << std::endl;
}

// Generate a vector of all valid successor states (neighbors)
std::vector<PuzzleState> PuzzleState::getSuccessors() const
{
    std::vector<PuzzleState> successors;

    // 1. Find the location of the blank tile
    std::pair<int, int> blankPos = findBlank();
    int blankRow = blankPos.first;
    int blankCol = blankPos.second;

    // Safety check in case the board is invalid
    if (blankRow == -1) {
        return successors; // Return empty vector
    }

    // 2. Define the 4 possible moves using delta-coordinates for {row, col}
    //    Moves:      Up,      Down,    Left,     Right
    const int dr[] = {-1,       1,       0,        0};
    const int dc[] = { 0,       0,      -1,        1};

    // 3. Iterate through each of the 4 potential moves
    for (int i = 0; i < 4; ++i)
    {
        int newRow = blankRow + dr[i];
        int newCol = blankCol + dc[i];

        // 4. Check if the new position is within the 3x3 grid
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3)
        {
            // If the move is valid, create the new state

            // a. Create a copy of the current board
            std::vector<std::vector<int>> newBoard = this->board;

            // b. Swap the blank tile with the adjacent tile
            std::swap(newBoard[blankRow][blankCol], newBoard[newRow][newCol]);

            // c. Create a new PuzzleState from the modified board and add it to our list
            successors.push_back(PuzzleState(newBoard));
        }
    }

    return successors;
}