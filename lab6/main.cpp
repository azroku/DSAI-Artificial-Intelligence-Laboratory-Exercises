#include <iostream>
#include <sstream>
#include "GameState.h"
#include "AIPlayer.h"

bool getHumanMove(int &row, int &col) {
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    if (!(ss >> row >> col)) {
        std::cout << "Invalid input: must use numbers 0, 1, or 2\n";
        return false;
    }
    if (row < 0 || row > 2 || col < 0 || col > 2) {
        std::cout << "Invalid input: numbers must be 0, 1, or 2\n";
        return false;
    }
    return true;
}

int main() {
    GameState game;
    char humanPlayer;
    
    std::cout << "Welcome to Tic-Tac-Toe!\n";
    std::cout << "Do you want to be X or O? ";
    std::cin >> humanPlayer;
    humanPlayer = toupper(humanPlayer);
    while (humanPlayer != PLAYER_X && humanPlayer != PLAYER_O) {
        std::cout << "Invalid choice. Choose X or O: ";
        std::cin >> humanPlayer;
        humanPlayer = toupper(humanPlayer);
    }

    std::cin.ignore();
    char aiPlayerChar = (humanPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    AIPlayer ai(aiPlayerChar);

    char currentPlayer = PLAYER_X;
    while (true) {
        game.printBoard();
        char winner = game.checkWinner();
        if (winner != ' ') {
            if (winner == 'D') std::cout << "It's a draw!\n";
            else if (winner == humanPlayer) std::cout << "You win!\n";
            else std::cout << "AI wins!\n";
            break;
        }

        if (currentPlayer == humanPlayer) {
            int row, col;
            bool valid = false;
            while (!valid) {
                std::cout << "Enter your move (row and column: 0 1 2): ";
                valid = getHumanMove(row, col);
                if (valid && !game.makeMove(row, col, humanPlayer)) {
                    std::cout << "Cell is already occupied. Try again.\n";
                    valid = false;
                }
            }
        } else {
            Move bestMove = ai.findBestMove(game);
            game.makeMove(bestMove.row, bestMove.col, aiPlayerChar);
            std::cout << "AI plays: " << bestMove.row << " " << bestMove.col << "\n";
        }

        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    game.printBoard();
    return 0;
}