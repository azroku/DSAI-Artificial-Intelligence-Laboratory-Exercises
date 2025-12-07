#include "AIPlayer.h"
#include <limits>

AIPlayer::AIPlayer(char aiMarker) {
    this->aiMarker = aiMarker;
    opponentMarker = (aiMarker == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

Move AIPlayer::findBestMove(GameState& state) {
    return minimax_alpha_beta(state, aiMarker, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

Move AIPlayer::minimax_alpha_beta(GameState& state, char player, int alpha, int beta) {
    char winner = state.checkWinner();
    Move bestMove;

    if (winner == aiMarker) { bestMove.score = 10; return bestMove; }
    if (winner == opponentMarker) { bestMove.score = -10; return bestMove; }
    if (winner == 'D') { bestMove.score = 0; return bestMove; }

    std::vector<std::pair<int,int>> moves = state.getAvailableMoves();

    if (player == aiMarker) {
        bestMove.score = std::numeric_limits<int>::min();
        for (auto move : moves) {
            state.makeMove(move.first, move.second, player);
            Move currentMove = minimax_alpha_beta(state, opponentMarker, alpha, beta);
            state.undoMove(move.first, move.second);

            if (currentMove.score > bestMove.score) {
                bestMove.score = currentMove.score;
                bestMove.row = move.first;
                bestMove.col = move.second;
            }
            alpha = std::max(alpha, bestMove.score);
            if (alpha >= beta) break;
        }
    } else {
        bestMove.score = std::numeric_limits<int>::max();
        for (auto move : moves) {
            state.makeMove(move.first, move.second, player);
            Move currentMove = minimax_alpha_beta(state, aiMarker, alpha, beta);
            state.undoMove(move.first, move.second);

            if (currentMove.score < bestMove.score) {
                bestMove.score = currentMove.score;
                bestMove.row = move.first;
                bestMove.col = move.second;
            }
            beta = std::min(beta, bestMove.score);
            if (alpha >= beta) break;
        }
    }

    return bestMove;
}