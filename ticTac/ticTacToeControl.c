/*
 * ticTacToeControl.c
 *
 *  Created on: Oct 21, 2015
 *      Author: skguth10
 */
#include "ticTacToeControl.h"

enum TTT_States {TTT_init_st, TTT_draw_instructions_st, TTT_let_user_read_st, TTT_wait_first_touch_st,
    TTT_wait_for_settle_st, TTT_computer_play_st, TTT_user_play_st, TTT_wait_touch_st, TTT_end_st
} TTT_State;

#define TTT_READ_EXPIRED 30//give player time to read instructions. give them 5 seconds to read
#define TTT_FIRST_TOUCH_EXPIRED 30//On first move, give player to go first or not. AKA be X. Give them 5 seconds
#define ADTIMER_EXPIRE 1//allows touch screen time to settle before reading. needs 50 m/s

static uint8_t readTimer = 0; //counts how long we give player time to read instructions
static uint8_t firstTouchTimer = 0; //counts how long we give the player the chance to go first (be X)
static uint8_t adTimer = 0; //counts up to 50m/s. it allow the touch to settle

//displayed on screen to show user what to do
#define INSTRUCTIONS "Touch Board to Play as X\n           -OR- \n    Wait for computer\n      and play as O"
#define TEXT_SIZE 2 //how big do we want to show it?
#define INSTRUCTIONS_LOCATION_X TEXT_SIZE*6//where on board, X, half way down
#define INSTRUCTIONS_LOCATION_Y DISPLAY_HEIGHT/3 //where on board, Y

static minimax_board_t currentBoard = {}; //used to keep track of whether its a full board or not. init to 0

bool playerX = false; //declare if player is going to play first (play as X)
bool invalidMoveFlag = false; //if the player tries to go in a wrong spot
static uint32_t newBTNValue = 0; //the value of button 0. used in reset

static uint8_t controlTouchRow = 0; //used to define where the player touched
static uint8_t controlTouchColumn = 0; //and where their move will be drawn

void debugStatePrint() {
    static TTT_States previousState;
    static bool firstPass = true;
    // Only print the message if:
    // 1. This the first pass and the value for previousState is unknown.
    // 2. previousState != currentState - this prevents reprinting the same state name over and over.
    if (previousState != TTT_State || firstPass) {
        firstPass = false;                // previousState will be defined, firstPass is false.
        previousState = TTT_State;     // keep track of the last state that you were in.

        switch(TTT_State) {            // This prints messages based upon the state that you were in.
        case TTT_init_st:
            printf("TTT_init_st\n\r");
            break;
        case TTT_draw_instructions_st:
            printf("TTT_draw_instructions_st\n\r");
            break;
        case TTT_let_user_read_st:
            printf("TTT_let_user_read_st\n\r");
            break;
        case TTT_wait_first_touch_st:
            printf("TTT_wait_first_touch_st\n\r");
            break;
        case TTT_wait_for_settle_st:
            printf("TTT_wait_for_settle_st\n\r");
            break;
        case TTT_computer_play_st:
            printf("TTT_computer_play_st\n\r");
            break;
        case TTT_user_play_st:
            printf("TTT_user_play_st\n\r");
            break;
        case TTT_wait_touch_st:
            printf("TTT_wait_touch_st\n\r");
            break;
        case TTT_end_st:
            printf("TTT_end_st\n\r");
        }
    }
}

//checks if button zero was pressed, if it is, changes to reset state
//
void ticTacToeControl_checkResetBTN()
{
    if ((newBTNValue & BUTTONS_BTN0) == BUTTONS_BTN0) //BTN0 was pressed
    {
        TTT_State = TTT_wait_first_touch_st; //return to beginning
        minimax_initBoard(&currentBoard); //reset board
        firstTouchTimer = 0; //reset wait timer
        ticTacToeDisplay_reset(); //reset board display
    }
}

//checks if game is over
bool ticTacToeControl_checkEndGame(minimax_board_t *board, bool player)
{
    int16_t score = minimax_computeBoardScore(board, player); //get board score
    return minimax_isGameOver(score); //return if game is over
}

void ticTacToeControl_tick() //actions and transitions of state machine
{
    debugStatePrint(); //print state for debugging

    switch (TTT_State) //state machine actions
    {
    case TTT_init_st: //inital entry state
        readTimer = 0; //initialize read timer
        firstTouchTimer = 0; //set to zero
        adTimer = 0; //set to zerio
        break;
    case TTT_draw_instructions_st:
        //draw print the instructions
        display_setTextSize(TEXT_SIZE); //how big
        display_setTextColor(DISPLAY_WHITE);//set the color
        display_setCursor(INSTRUCTIONS_LOCATION_X, INSTRUCTIONS_LOCATION_Y);
        display_println(INSTRUCTIONS);//print instructions
        break;
    case TTT_let_user_read_st:
        readTimer++; //increase read timer
        break;
    case TTT_wait_first_touch_st:
        firstTouchTimer++; //increment first touch wait timer
        break;
    case TTT_wait_for_settle_st:
        adTimer++; //increment timer for settling
        break;
    case TTT_computer_play_st:
        if (playerX) //computer is O
        {
            minimax_computeNextMove(&currentBoard, false, &controlTouchRow, &controlTouchColumn); //its always computing computers move, so just set the bool to false
            ticTacToeDisplay_drawO(controlTouchRow, controlTouchColumn);
            //mark that board space
            currentBoard.squares[controlTouchRow][controlTouchColumn] = MINIMAX_OPPONENT_SQUARE;
        }

        else //computer went first, is X
        {
            minimax_computeNextMove(&currentBoard, false, &controlTouchRow, &controlTouchColumn); //its always computing computers move, so just set the bool to false
            ticTacToeDisplay_drawX(controlTouchRow, controlTouchColumn);
            //mark that board space
            currentBoard.squares[controlTouchRow][controlTouchColumn] = MINIMAX_OPPONENT_SQUARE;
        }

        break;
    case TTT_user_play_st:
        //read button value here, and check in transitions
        newBTNValue = buttons_read();

        invalidMoveFlag = false; // reset the flag

        //reset wait for settle timer
        adTimer = 0;

        if (playerX) //player went first, is X
        {
            ticTacToeDisplay_touchScreenComputeBoardRowColumn(&controlTouchRow, &controlTouchColumn);
            if(currentBoard.squares[controlTouchRow][controlTouchColumn] == MINIMAX_EMPTY_SQUARE) //check square is empty
            {
                ticTacToeDisplay_drawX(controlTouchRow, controlTouchColumn); //draw the x
                //mark that board space
                currentBoard.squares[controlTouchRow][controlTouchColumn] = MINIMAX_PLAYER_SQUARE; //mark move in board
            }
            else
                invalidMoveFlag = true; //player tried to go in occupied square
        }

        else //player is O
        {
            ticTacToeDisplay_touchScreenComputeBoardRowColumn(&controlTouchRow, &controlTouchColumn);
            if(currentBoard.squares[controlTouchRow][controlTouchColumn] == MINIMAX_EMPTY_SQUARE) //check square is empty
            {
                ticTacToeDisplay_drawO(controlTouchRow, controlTouchColumn); //draw the O
                //mark that board space
                currentBoard.squares[controlTouchRow][controlTouchColumn] = MINIMAX_PLAYER_SQUARE; //mark move in board
            }
            else
                invalidMoveFlag = true; //player tried to go in occupied square
        }
        break;
    case TTT_wait_touch_st:
        //just wait
    case TTT_end_st:
        //read button value here, and check in transitions
        //only reset at end game
        newBTNValue = buttons_read();

    default:
        break;
    }



    //////////////////////////////////////////////////////////////////////////////////////////////////



    switch (TTT_State) //state machine transitions
    {
    case TTT_init_st: //inital entry state
        TTT_State = TTT_draw_instructions_st; //move to instructions state
        break;
    case TTT_draw_instructions_st:
        TTT_State = TTT_let_user_read_st; //go straight to reading after instructions are drawn
        break;
    case TTT_let_user_read_st:
        if (readTimer == TTT_READ_EXPIRED)
        {
            TTT_State = TTT_wait_first_touch_st; //wait for touching
            ticTacToeDisplay_reset();//draw empty board. mealey action
        }
        break;
    case TTT_wait_first_touch_st:
    {
        if (firstTouchTimer == TTT_FIRST_TOUCH_EXPIRED) //if the wait is expired
        {
            //computer goes first. computer is X
            TTT_State = TTT_computer_play_st;
            playerX = false;
        }
        else if (display_isTouched()) //human touched a spot
        {
            //human goes first. is X
            TTT_State = TTT_wait_for_settle_st;
            playerX = true;
        }
    }
    break;
    case TTT_wait_for_settle_st:
        if (adTimer == ADTIMER_EXPIRE) //gave touch screen time to settle
        {
            TTT_State = TTT_user_play_st; //go to user play
        }
        break;
    case TTT_computer_play_st:
        // if board full, go to end game
        if (ticTacToeControl_checkEndGame(&currentBoard, false)) //pass in the board, does matter what the bool is, just need an endgame score
            TTT_State = TTT_end_st;
        else
            TTT_State = TTT_wait_touch_st; //go to place to wait for player to touch
        break;
    case TTT_user_play_st:
        if (ticTacToeControl_checkEndGame(&currentBoard, true)) //pass in the board, does matter what the bool is, just need an endgame score
            TTT_State = TTT_end_st;

        else if(invalidMoveFlag) //player tried to go in occupied space
        {
            TTT_State = TTT_wait_touch_st; //wait for another touch
        }

        else
            TTT_State = TTT_computer_play_st; //go to computer move


        break;
    case TTT_wait_touch_st:
        if (ticTacToeControl_checkEndGame(&currentBoard, playerX)) //pass in the board, does matter what the bool is, just need an endgame score
                    TTT_State = TTT_end_st;
        else if (display_isTouched()) //done waiting
        {
            TTT_State = TTT_wait_for_settle_st; //go settle state
        }
        break;
    case TTT_end_st:
        ticTacToeControl_checkResetBTN(); //check if button has been pressed,
        break;
    default:
        printf("ERROR: missed transition\n\r");
        break;
    }
}

