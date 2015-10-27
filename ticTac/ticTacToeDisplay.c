/*
 * ticTacToeDisplay2.c
 *
 *  Created on: Oct 21, 2015
 *      Author: skguth10
 */

#include "ticTacToeDisplay.h"

//TTTD is short for TICTACTOEDISPLAY
#include "src/buttons/buttons.h"
#include "src/buttons/switches.h"
#include <stdio.h>

#include <stdint.h>
#include "supportFiles/utils.h" // for testing purposes

#define TTTD_TEST_TERMINATED "Test Terminated"

//----------------------------------------------------------------------------------
//DRAW LINES

#define TTTD_VERTICAL_LINE_0 (DISPLAY_WIDTH/3)//seperates column 0 and 1
#define TTTD_VERTICAL_LINE_1 ((DISPLAY_WIDTH*2)/3)//seperates column 1 and 2

#define TTTD_HORIZONTAL_LINE_0 (DISPLAY_HEIGHT/3)//seperates row 0 and 1
#define TTTD_HORIZONTAL_LINE_1 ((DISPLAY_HEIGHT*2)/3)//seperates row 1 and 2

//----------------------------------------------------------------------------------
//DRAW X
#define TTTD_DRAW_X_UPPERLEFT_X 20 //left hand corner of X
#define TTTD_DRAW_X_UPPERLEFT_Y 10 //left hand corner of X

#define TTTD_DRAW_X_LOWERLEFT_X 20 //lower left corner of X
#define TTTD_DRAW_X_LOWERLEFT_Y (DISPLAY_HEIGHT/3)-10 //lower left corner of X

#define TTTD_DRAW_X_UPPERRIGHT_X (DISPLAY_WIDTH/3)-10 //uper right corner
#define TTTD_DRAW_X_UPPERRIGHT_Y 10 //uper right corner

#define TTTD_DRAW_X_LOWERRIGHT_X (DISPLAY_WIDTH/3)-10 //lower right
#define TTTD_DRAW_X_LOWERRIGHT_Y (DISPLAY_HEIGHT/3)-10//lower right

//#define
//----------------------------------------------------------------------------------
//DRAW O

//these 2 points are the middle of the first square, we'll simply "add a square for each row we're in
#define TTTD_DRAW_CIRCLE_XBASE ((DISPLAY_WIDTH/3)/2) //half way between the left edge and vert line 0
#define TTTD_DRAW_CIRCLE_YBASE ((DISPLAY_HEIGHT/6)) //half way between top edge and the horizontial line 0
#define TTTD_BUFFER_X (DISPLAY_WIDTH/3) //
#define TTTD_BUFFER_Y (DISPLAY_HEIGHT/3)

#define TTTD_CIRCLE_RADIUS 30 //radius of circles



//----------------------------------------------------------------------------------
// ROW & COLUMN
static uint8_t TTTD_currentColumn = 0; //modified by ComputerBoardRowColumn, passed to draw X and draw O
static uint8_t TTTD_currentRow = 0;

static int16_t TTTD_touchCursorX = 0; //where's the cursor
static int16_t TTTD_touchCursorY = 0;
static uint8_t TTTD_touchPressure = 0; //I don't think i'll actually use this, but I think get

//These will worry about whether a spot on the board has already been drawn on
//if it is occupied, we can't draw there
#define TTTD_BOARD_SIZE 3

#define TTTD_ROW_0 0 //these don't need to be defined, but since there is a row2 and column 2 we'll add to be consistent
#define TTTD_ROW_1 1
#define TTTD_ROW_2 2

#define TTTD_COLUMN_0 0 //#define for consistency
#define TTTD_COLUMN_1 1
#define TTTD_COLUMN_2 2

void ticTacToeDisplay_drawBoardLines()
{
    display_drawFastVLine(TTTD_VERTICAL_LINE_0, 0, DISPLAY_HEIGHT, DISPLAY_CYAN); //draws a line across the board. seperates column 0 and 1
    display_drawFastVLine(TTTD_VERTICAL_LINE_1, 0, DISPLAY_HEIGHT, DISPLAY_CYAN); //seperates column 1 and 2

    display_drawFastHLine(0, TTTD_HORIZONTAL_LINE_0, DISPLAY_WIDTH, DISPLAY_CYAN); //seperates row 0 and 1
    display_drawFastHLine(0, TTTD_HORIZONTAL_LINE_1, DISPLAY_WIDTH, DISPLAY_CYAN); //seperates row 1 and 2
}

void ticTacToeDisplay_reset() //clears display and redraws the lines
{
    display_fillScreen(DISPLAY_BLACK); //clear
    ticTacToeDisplay_drawBoardLines(); //redraw
}
// Inits the tic-tac-toe display, draws the lines that form the board.
void ticTacToeDisplay_init()
{
    display_init(); //set up display
    ticTacToeDisplay_reset();
}

// Draws an X at the specified row and column.
void ticTacToeDisplay_drawX(uint8_t row, uint8_t column)
{
        //starts at specific point in row0column0, but will move over a row as necessay
        //draws the first line, going from upperleft to lower right
        display_drawLine(TTTD_DRAW_X_UPPERLEFT_X + TTTD_BUFFER_X * column,TTTD_DRAW_X_UPPERLEFT_Y + TTTD_BUFFER_Y * row, TTTD_DRAW_X_LOWERRIGHT_X + TTTD_BUFFER_X * column, TTTD_DRAW_X_LOWERRIGHT_Y + TTTD_BUFFER_Y * row, DISPLAY_CYAN);
        //draws second line, going from upper right to lower left
        display_drawLine(TTTD_DRAW_X_UPPERRIGHT_X + TTTD_BUFFER_X * column,TTTD_DRAW_X_UPPERRIGHT_Y + TTTD_BUFFER_Y * row, TTTD_DRAW_X_LOWERLEFT_X + TTTD_BUFFER_X * column, TTTD_DRAW_X_LOWERLEFT_Y + TTTD_BUFFER_Y * row, DISPLAY_CYAN);
}

// Draws an O at the specified row and column.
void ticTacToeDisplay_drawO(uint8_t row, uint8_t column)
{
        //this is cool we start in the center of row0column0, and move over to the centers based on row & column
        display_drawCircle(TTTD_DRAW_CIRCLE_XBASE + TTTD_BUFFER_X * column, TTTD_DRAW_CIRCLE_YBASE + TTTD_BUFFER_Y * row, TTTD_CIRCLE_RADIUS, DISPLAY_CYAN);
}

// After a touch has been detected and after the proper delay, this sets the row and column arguments
// according to where the user touched the board.
void ticTacToeDisplay_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column)
{
    //utils_msDelay(50); //wait for touch to settle
    display_clearOldTouchData();
    display_getTouchedPoint(&TTTD_touchCursorX, &TTTD_touchCursorY, &TTTD_touchPressure); //need to relay this info to the Cursors
    //printf("X:%d and Y:%d\n\r", TTTD_touchCursorX, TTTD_touchCursorY);

    //------------------------------------------------------------
    //COMPUTE COLUMN
    if (TTTD_touchCursorX < TTTD_VERTICAL_LINE_0) //IS IT IN COLUMN 0
    {
        *column = TTTD_COLUMN_0;
    }

    else if(TTTD_touchCursorX <= TTTD_VERTICAL_LINE_1) //is it in COLUMN 1
    {
        *column = TTTD_COLUMN_1;
    }

    else if(TTTD_touchCursorX >= TTTD_VERTICAL_LINE_1) //is it in COLUMN 2
    {
        *column = TTTD_COLUMN_2;
    }
    //------------------------------------------------------------
    //COMPUTE ROW
    if (TTTD_touchCursorY < TTTD_HORIZONTAL_LINE_0) //IS IT IN ROW 0
    {
        *row = TTTD_ROW_0;
    }

    else if(TTTD_touchCursorY <= TTTD_HORIZONTAL_LINE_1) //is it in ROW 1
    {
        *row = TTTD_ROW_1;
    }

    else if(TTTD_touchCursorY >= TTTD_HORIZONTAL_LINE_1) //is it in ROW 2
    {
        *row = TTTD_ROW_2;
    }
    //printf("row:%d and column:%d\n\r", *row, *column);
}

// Runs a test of the display. Does the following.
// Draws the board. Each time you touch one of the screen areas, the screen will paint
// an X or an O, depending on whether switch 0 (SW0) is slid up (O) or down (X).
// When BTN0 is pushed, the screen is cleared. The test terminates when BTN1 is pushed.
void ticTacToeDisplay_runTest()
{
    ticTacToeDisplay_init();
    buttons_init();
    switches_init();
    int32_t switchValue = 0;
    uint32_t newBTNValue = 0;
    while(1)
    {
        switchValue = switches_read();
        newBTNValue = buttons_read();

        if (display_isTouched())
        {
            if ((switchValue & SWITCHES_SW0) == SWITCHES_SW0) //switch is up
            {
                //draw0
                ticTacToeDisplay_touchScreenComputeBoardRowColumn(&TTTD_currentRow, &TTTD_currentColumn);

                ticTacToeDisplay_drawO(TTTD_currentRow, TTTD_currentColumn);
            }
            else //switch is down
            {
                //drawx
                ticTacToeDisplay_touchScreenComputeBoardRowColumn(&TTTD_currentRow, &TTTD_currentColumn);
                ticTacToeDisplay_drawX(TTTD_currentRow, TTTD_currentColumn);
            }
        }

        if ((newBTNValue & BUTTONS_BTN0) == BUTTONS_BTN0) //button 0 pressed
        {
            ticTacToeDisplay_reset(); //clear screen & redraw lines
        }
        if ((newBTNValue & BUTTONS_BTN1) == BUTTONS_BTN1) //button 1 pressed
        {
            display_fillScreen(DISPLAY_BLACK); //clear screen
            display_setTextColor(DISPLAY_CYAN); //set text color
            display_println(TTTD_TEST_TERMINATED);//print out that its over
            break; //exit the loop
        }
    }
}

// This will draw the four board lines.








