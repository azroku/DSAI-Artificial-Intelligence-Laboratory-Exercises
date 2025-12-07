#include "GameState.h"

GameState::GameState() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = EMPTY_CELL;
}

void GameState::printBoard() const {
    std::cout << "-------------\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 3; ++j) {
            std::cout << board[i][j] << " | ";
        }
        std::cout << "\n-------------\n";
    }
}

bool GameState::makeMove(int row, int col, char player) {
    if (row < 0 || row > 2 || col < 0 || col > 2)
        return false;
    if (board[row][col] != EMPTY_CELL)
        return false;
    board[row][col] = player;
    return true;
}

void GameState::undoMove(int row, int col) {
    board[row][col] = EMPTY_CELL;
}

std::vector<std::pair<int, int>> GameState::getAvailableMoves() const {
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == EMPTY_CELL)
                moves.push_back({i, j});
    return moves;
}

char GameState::checkWinner() const {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != EMPTY_CELL &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];

        if (board[0][i] != EMPTY_CELL &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }

    if (board[0][0] != EMPTY_CELL &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] != EMPTY_CELL &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    bool full = true;
    for (int i = 0; i < 3 && full; ++i)
        for (int j = 0; j < 3 && full; ++j)
            if (board[i][j] == EMPTY_CELL)
                full = false;

    if (full) return 'D';

    return ' ';
}