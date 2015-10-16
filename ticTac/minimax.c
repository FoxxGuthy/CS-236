/*
 * minimax.c
 *
 *  Created on: Oct 13, 2015
 *      Author: skguth10
 */
#include "minimax.h"

minimax_move_t choice; //will store which move holds the highest score


// This routine is not recursive but will invoke the recursive minimax function.
// It computes the row and column of the next move based upon:
// the current board,
// the player. true means the computer is X. false means the computer is O.
void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column)
{
    //entry
    //calls minimax
}

// Determine that the game is over by looking at the score.
bool minimax_isGameOver(minimax_score_t score)
{
    return true;
}

// Returns the score of the board, based upon the player and the board.
// This returns one of 4 values: MINIMAX_PLAYER_WINNING_SCORE,
// MINIMAX_OPPONENT_WINNING_SCORE, MINIMAX_DRAW_SCORE, MINIMAX_NOT_ENDGAME
int16_t minimax_computeBoardScore(minimax_board_t* board, bool player)
{
    return 0;
}

// Init the board to all empty squares.
void minimax_initBoard(minimax_board_t* board)
{

}

uint8_t minimax_findScore(minimax_board_t* board, bool player)
{

}
