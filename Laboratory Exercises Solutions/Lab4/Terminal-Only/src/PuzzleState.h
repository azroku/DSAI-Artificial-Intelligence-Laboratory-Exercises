#pragma once
#include <vector>
#include <string>

class PuzzleState {
public:
    // The 3x3 board. 0 represents the blank tile.
    std::vector<std::vector<int>> board;

    // Constructor to initialize from a 3x3 vector
    PuzzleState(const std::vector<std::vector<int>>& initialBoard);

    // Find the position of the blank tile (0)
    std::pair<int, int> findBlank() const;

    // Generate a vector of all valid successor states
    std::vector<PuzzleState> getSuccessors() const;

    // Overload operator< to allow this state to be a key in std::map or std::set
    bool operator<(const PuzzleState& other) const;
    
    // (Optional but helpful) Overload operator== for easy comparison
    bool operator==(const PuzzleState& other) const;

    // (Optional but helpful) A function to print the board to the console
    void print() const;
};