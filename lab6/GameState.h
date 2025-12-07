#pragma once
#include <vector>
#include <iostream>

const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY_CELL = ' ';

class GameState {
private:
    char board[3][3];

public:
    GameState();
    void printBoard() const;
    bool makeMove(int row, int col, char player);
    void undoMove(int row, int col);
    std::vector<std::pair<int, int>> getAvailableMoves() const;
    char checkWinner() const;
};