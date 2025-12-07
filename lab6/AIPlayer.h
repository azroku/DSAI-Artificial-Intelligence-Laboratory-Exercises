#pragma once
#include "GameState.h"
#include <vector>

struct Move {
    int row = -1;
    int col = -1;
    int score = 0;
};

class AIPlayer {
public:
    AIPlayer(char aiMarker);
    Move findBestMove(GameState& state);

private:
    char aiMarker;
    char opponentMarker;
    Move minimax_alpha_beta(GameState& state, char player, int alpha, int beta);
};