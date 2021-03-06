#include "minimax.h"
#include <stdio.h>

#define MINIMAX_BOARD_SIZE 9
minimax_move_t choice;
#define ROW0 0 //dont need this one, but we'll be consistent
#define ROW1 1
#define ROW2 2 //can't have magic numbers
#define COL0 0 //column 0
#define COL1 1 //column 1
#define COL2 2 //get rid of that
//recursive fuction

int8_t minimax(minimax_board_t *board, bool player, uint8_t depth)
{
    //use !player because it will return -1 which means the game is not over
    minimax_score_t score = minimax_computeBoardScore(board, !player);

    if (minimax_isGameOver(score)) //game is over
    {
        return minimax_computeBoardScore(board, !player);
    }

    minimax_score_t scores[MINIMAX_BOARD_SIZE]; //store the of each move, then we;ll do the move with the highest score
    minimax_move_t moves[MINIMAX_BOARD_SIZE];

    uint8_t moveIndex = 0; //for tracking where we are in the move and score arrays

    for (uint8_t i = 0; i < MINIMAX_BOARD_ROWS; i++) //iterate thru rows and columns
    {
        for (uint8_t j = 0; j < MINIMAX_BOARD_COLUMNS; j++)
        {
            if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE) //if its empty, assign to player or computer
            {
                if (player) //if player assign to player
                    board->squares[i][j] = MINIMAX_PLAYER_SQUARE;
                else //assigned to computer
                    board->squares[i][j] = MINIMAX_OPPONENT_SQUARE;

                score = minimax(board, !player, depth+1); //update score

                //add score to move-score table
                scores[moveIndex] = score;
                //add move to move-score table
                moves[moveIndex].column = j;
                moves[moveIndex].row = i;
                moveIndex++;

                //undo change to board for next iteration
                board->squares[i][j] = MINIMAX_EMPTY_SQUARE;
            }
        }
    }
    minimax_score_t tempScore = scores[0];
    uint8_t index = 0;
    if (player)
    {
        //choice <= get the move with the highest score in the move - score table.
        //score <= highest score in the move - score table.
        for (uint8_t i = 0; i < moveIndex; i++)
        {
            if (scores[i] > tempScore) //if the score in array is bigger, make that the new highscore and change move to that spot
            {
                tempScore = scores[i];
                index = i; //we don't set choice here because if its equal, then choice doesn't get reset
            }
        }
        choice.row = moves[index].row;
        choice.column = moves[index].column;
        score = scores[index];
    }
    else
    {
        // choice = move w/ lowest score
        // score == lowest score
        for (uint8_t i = 0; i < moveIndex; i++)
        {
            if (scores[i] < tempScore) //if the score in array is smaller, make that the new low and change move to that spot
            {
                tempScore = scores[i]; //set score
                index = i; //we don't set choice here because if its equal, then choice doesn't get reset
            }
        }
        choice.row = moves[index].row;
        choice.column = moves[index].column;
        score = scores[index];
    }
    return score;
}

void minimax_initBoard(minimax_board_t* board)
{
    for (uint8_t i = 0; i < MINIMAX_BOARD_ROWS; i++) //iterate thru rows
    {
        for (uint8_t j = 0; j < MINIMAX_BOARD_COLUMNS; j++) //iterate thru columns
        {
            board->squares[i][j] = MINIMAX_EMPTY_SQUARE; //set all squares equal to empty
        }
    }
}

//looks to see if the board for 3 in a row
bool minimax_rowWin(minimax_board_t* board, bool player)
{
    uint8_t playerNumber;
    if (player) //are we looking for player or computer win?
        playerNumber = MINIMAX_PLAYER_SQUARE;

    else
        playerNumber = MINIMAX_OPPONENT_SQUARE;

    return //return if any of the rows all equal playerNumber
        (board->squares[ROW0][COL0] == playerNumber && board->squares[ROW0][COL1] == playerNumber && board->squares[ROW0][COL2] == playerNumber) || //row0
        (board->squares[ROW1][COL0] == playerNumber && board->squares[ROW1][COL1] == playerNumber && board->squares[ROW1][COL2] == playerNumber) || //row1
        (board->squares[ROW2][COL0] == playerNumber && board->squares[ROW2][COL1] == playerNumber && board->squares[ROW2][COL2] == playerNumber); //row2
}

//looks for 3 in a single column
bool minimax_columnWin(minimax_board_t* board, bool player)
{
    uint8_t playerNumber;
    if (player) //are we looking for player or computer win?
        playerNumber = MINIMAX_PLAYER_SQUARE;

    else
        playerNumber = MINIMAX_OPPONENT_SQUARE;

    return //return if any of the columns all equal playerNumber
        (board->squares[ROW0][COL0] == playerNumber && board->squares[ROW1][COL0] == playerNumber && board->squares[ROW2][COL0] == playerNumber) || //column0
        (board->squares[ROW0][COL1] == playerNumber && board->squares[ROW1][COL1] == playerNumber && board->squares[ROW2][COL1] == playerNumber) || //column1
        (board->squares[ROW0][COL2] == playerNumber && board->squares[ROW1][COL2] == playerNumber && board->squares[ROW2][COL2] == playerNumber); //column2
}

//checks for diagnol win
bool  minimax_diagonalWin(minimax_board_t* board, bool player)
{
    uint8_t playerNumber;
    if (player) //are we looking for player or computer win?
        playerNumber = MINIMAX_PLAYER_SQUARE; //look for squares belonging to player

    else
        playerNumber = MINIMAX_OPPONENT_SQUARE; //look for squares belonging to computer

    return
        (board->squares[ROW0][COL0] == playerNumber &&
            board->squares[ROW1][COL1] == playerNumber &&
            board->squares[ROW2][COL2] == playerNumber) || //upper left to lower right
        (board->squares[ROW0][COL2] == playerNumber &&
            board->squares[ROW1][COL1] == playerNumber &&
            board->squares[ROW2][COL0] == playerNumber); //upper right to lower left
}

//checks for draw, but only call after we checking for win
//simply looks for empty squares, returns false if found
bool minimax_draw(minimax_board_t* board)
{
    for (uint8_t i = 0; i < MINIMAX_BOARD_ROWS; i++) //iterate thru rows and columns
    {
        for (uint8_t j = 0; j < MINIMAX_BOARD_COLUMNS; j++)
        {
            if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE)
            {
                return false; //there's an empty square, so can't be draw yet
            }
        }
    }
    return true; //went thru the board and didn't find empty square, its a draw if its not a win
}

//entry function, calls minimax recursive
void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column)
{
    minimax(board, player, 0);
    *row = choice.row;
    *column = choice.column;
}

bool minimax_isGameOver(minimax_score_t score)
{
    if (score == MINIMAX_NOT_ENDGAME) //is the score -1? its not over baby
        return false;
    else //else its game over sucka
        return true;
}

int16_t minimax_computeBoardScore(minimax_board_t* board, bool player)
{
    if (minimax_rowWin(board, player) || minimax_columnWin(board, player) || minimax_diagonalWin(board, player)) //check for a win
    {
        if (player)
        {
            return MINIMAX_PLAYER_WINNING_SCORE; //player won. return -10
        }
        else
            return MINIMAX_OPPONENT_WINNING_SCORE; //computer won. return 10
    }

    if (minimax_draw(board)) // if its a draw return 0
        return MINIMAX_DRAW_SCORE;

    return MINIMAX_NOT_ENDGAME;
}

