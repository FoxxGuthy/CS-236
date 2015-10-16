/*SETH GUTHRIE ECEN 330
 * clockDisplay.c
 *
 *  Created on: Sep 28, 2015
 *      Author: skguth10
 */
#include "clockDisplay.h"





#define CLOCK_DISPLAY_TEXT_SIZE 3 //our clock must be able to resize, all based on this value


//the value of the hours (or the whole string) on the x axis
//this is 4 chars left of the middle of the x axis
#define CLOCK_DISPLAY_HOURS_POSITION_X (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE - 4*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH)


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//TRIANGLE POSITIONING



#define CLOCK_DISPLAY_TOP_TRI_HEIGHT (CLOCK_DISPLAY_NUMBERS_HEIGHT - (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT/3)) //the height of the bottom edge of the top tris
#define CLOCK_DISPLAY_TOP_TRI_POINT_HEIGHT (CLOCK_DISPLAY_NUMBERS_HEIGHT - (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT)) //height of point on top TRIS


//right of edge of the left most tri
#define CLOCK_DISPLAY_TRI0_RIGHT_EDGE (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE - 2*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH)

//left edge of the middle tri
#define CLOCK_DISPLAY_TRI1_LEFT_EDGE (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE - 1*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH)
//right edge of the middle tri
#define CLOCK_DISPLAY_TRI1_RIGHT_EDGE (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE + 1*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH)

//left edge of the right tri
#define CLOCK_DISPLAY_TRI2_LEFT_EDGE (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE + 2*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH)
//right edge of right most trie
#define CLOCK_DISPLAY_TRI2_RIGHT_EDGE (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE + 4*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH)

//the position on the a axis of the points of the triangles
#define CLOCK_DISPLAY_TRI0_POINT_X (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE - 3*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH) //useable for top and bottom
#define CLOCK_DISPLAY_TRI1_POINT_X (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE) //useable for top and bottom
#define CLOCK_DISPLAY_TRI2_POINT_X (CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE + 3*CLOCK_DISPLAY_TEXT_PIXEL_WIDTH) //useable for top and bottom


#define CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT (CLOCK_DISPLAY_SCREEN_HEIGHT_MIDDLE + (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT/3) + (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT/3) ) //the top edge of bottom tris
//so because the top of the chars is already included in the value of numbers height, the middle + half the height, we have the add the bottom and then the 1/3 down
#define CLOCK_DISPLAY_BOT_TRI_POINT_HEIGHT (CLOCK_DISPLAY_SCREEN_HEIGHT_MIDDLE + (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT/3) + (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CLOCK stuff
#define CLOCK_DISPLAY_MAX_SECONDS 60 //used for inc time by 1 seconds
#define CLOCK_DISPLAY_MAX_MINUTES 60
#define CLOCK_DISPLAY_MAX_HOURS 13

#define CLOCK_DISPLAY_TIME_FORMAT "%2hd:%02hd:%02hd" //this will be used for printing out the clock

static uint8_t hours = 12; //if you start this at zero, it'll break itself.
static uint8_t minutes = 59;
static uint8_t seconds = 58;

#define CLOCK_DISPLAY_ARRAY_SIZE 9 //the time array

static char newCompleteTime[CLOCK_DISPLAY_ARRAY_SIZE]; //the array in which time is built
static char oldCompleteTime[CLOCK_DISPLAY_ARRAY_SIZE]; //for comparing

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int16_t touchCursorX = 0; //where's your cursor boy? for inc/dec
static int16_t touchCursorY = 0;
static uint8_t touchPressure = 0; //I don't think i'll actually use this, but I think get

#define CLOCK_DISPLAY_SCREEN_THIRD0 (DISPLAY_WIDTH/3) //first border. seperates 1 & 2
#define CLOCK_DISPLAY_SCREEN_THIRD2 ((DISPLAY_WIDTH * 2)/3) //the second border. seperates 2 & 3

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INC/DEC



#define CLOCK_DISPLAY_INC 0 //Y values for runtest, setting the cursosrs
#define CLOCK_DISPLAY_DEC 240 //Y values for dec

#define CLOCK_DISPLAY_HOURS 0 //left side, should be hours, X
#define CLOCK_DISPLAY_MINUTES 160 //middle, should be minutes
#define CLOCK_DISPLAY_SECONDS 320 //right, should be seconds X

#define CLOCK_DISPLAY_HOURS_MIN_ROLL 1 //defines the bounds of hours
#define CLOCK_DISPLAY_HOURS_MAX_ROLL 12

#define CLOCK_DISPLAY_MINUTES_MIN_ROLL 0 //bounds of minutes
#define CLOCK_DISPLAY_MINUTES_MAX_ROLL 59

#define CLOCK_DISPLAY_SECONDS_MIN_ROLL 0 //bounds of seconds
#define CLOCK_DISPLAY_SECONDS_MAX_ROLL 59

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RUNTEST STUFF

#define CLOCK_DISPLAY_RUN_TEST_DELAY 100 //for the run test
#define CLOCK_DISPLAY_DELAY_1SEC 1000 //the longer delay for hours

#define CLOCK_DISPLAY_MORE_LOOPS 50 //loops for min/sec
#define CLOCK_DISPLAY_LESS_LOOPS 10 //loops for hours


void clockDisplay_draw()
{
    //set text size

    sprintf(newCompleteTime, CLOCK_DISPLAY_TIME_FORMAT, hours, minutes, seconds); //make the new string

    display_setTextSize(CLOCK_DISPLAY_TEXT_SIZE);

    uint8_t cursorX = CLOCK_DISPLAY_HOURS_POSITION_X; //where the potential change will occur
    for (int s = 0; s < CLOCK_DISPLAY_ARRAY_SIZE; s++)
    {
        if (oldCompleteTime[s] != newCompleteTime[s])
        {
            display_setCursor(cursorX, CLOCK_DISPLAY_NUMBERS_HEIGHT);
            display_setTextColor(DISPLAY_BLACK); //draw over the old chars in black
            display_println(oldCompleteTime[s]);

            display_setCursor(cursorX, CLOCK_DISPLAY_NUMBERS_HEIGHT); //reset the cursor
            display_setTextColor(DISPLAY_GREEN); //draw the new ones in green
            display_println(newCompleteTime[s]);

            oldCompleteTime[s] = newCompleteTime[s];
        }
        cursorX += CLOCK_DISPLAY_TEXT_PIXEL_WIDTH; //move the cursor over 1 char
    }

//    for(int i = 0; i < CLOCK_DISPLAY_ARRAY_SIZE; i++) //copy new to old
//    {
//        oldCompleteTime[i] = newCompleteTime[i];
//    }
}

void clockDisplay_init()  // Called only once - performs any necessary inits.
{
    display_init(); //init the display
    display_fillScreen(DISPLAY_BLACK); //blank the screen

    //we'll just draw the triangles here because we only ever want those drawn once
    //we have the triangles be half a char above the minutes

    //first up triangle (x,y, x1, y1, x2, y2) (left, right, top)
    display_fillTriangle(CLOCK_DISPLAY_HOURS_POSITION_X, CLOCK_DISPLAY_TOP_TRI_HEIGHT, CLOCK_DISPLAY_TRI0_RIGHT_EDGE, CLOCK_DISPLAY_TOP_TRI_HEIGHT, CLOCK_DISPLAY_TRI0_POINT_X, CLOCK_DISPLAY_TOP_TRI_POINT_HEIGHT, DISPLAY_GREEN);
    //draw hours down triangle
    display_fillTriangle(CLOCK_DISPLAY_HOURS_POSITION_X, CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT, CLOCK_DISPLAY_TRI0_RIGHT_EDGE, CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT, CLOCK_DISPLAY_TRI0_POINT_X, CLOCK_DISPLAY_BOT_TRI_POINT_HEIGHT, DISPLAY_GREEN);

    //draw up minutes triangle
    display_fillTriangle(CLOCK_DISPLAY_TRI1_LEFT_EDGE, CLOCK_DISPLAY_TOP_TRI_HEIGHT, CLOCK_DISPLAY_TRI1_RIGHT_EDGE, CLOCK_DISPLAY_TOP_TRI_HEIGHT, CLOCK_DISPLAY_TRI1_POINT_X, CLOCK_DISPLAY_TOP_TRI_POINT_HEIGHT, DISPLAY_GREEN);
    //draw down minutes triangle
    display_fillTriangle(CLOCK_DISPLAY_TRI1_LEFT_EDGE, CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT, CLOCK_DISPLAY_TRI1_RIGHT_EDGE, CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT, CLOCK_DISPLAY_TRI1_POINT_X, CLOCK_DISPLAY_BOT_TRI_POINT_HEIGHT, DISPLAY_GREEN);

    //draw up seconds tri
    display_fillTriangle(CLOCK_DISPLAY_TRI2_LEFT_EDGE, CLOCK_DISPLAY_TOP_TRI_HEIGHT, CLOCK_DISPLAY_TRI2_RIGHT_EDGE, CLOCK_DISPLAY_TOP_TRI_HEIGHT, CLOCK_DISPLAY_TRI2_POINT_X, CLOCK_DISPLAY_TOP_TRI_POINT_HEIGHT, DISPLAY_GREEN);
    // down seconds tri
    display_fillTriangle(CLOCK_DISPLAY_TRI2_LEFT_EDGE, CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT, CLOCK_DISPLAY_TRI2_RIGHT_EDGE, CLOCK_DISPLAY_BOTTOM_TRI_HEIGHT, CLOCK_DISPLAY_TRI2_POINT_X, CLOCK_DISPLAY_BOT_TRI_POINT_HEIGHT, DISPLAY_GREEN);

    //init the string
    sprintf(newCompleteTime, CLOCK_DISPLAY_TIME_FORMAT, hours, minutes, seconds); //make inital zeros & colons

    //draw the acutal numbers
    clockDisplay_draw();
}

void clockDisplay_updateTimeDisplay(bool forceUpdateAll)  // Updates the time display with latest time.
{
    clockDisplay_draw();
}

void clockDisplay_performIncDec()      // Performs the increment or decrement, depending upon the touched region.
{
    display_clearOldTouchData();
    display_getTouchedPoint(&touchCursorX, &touchCursorY, &touchPressure); //need to relay this info to the Cursors
    //printf(" X: %i\t",  touchCursorX);
    //printf("Y: %i \n\r", touchCursorY);

    if (touchCursorY > CLOCK_DISPLAY_SCREEN_HEIGHT_MIDDLE) //cursor or touch on bottom
    {
        //we're going to dec
        if (touchCursorX <= CLOCK_DISPLAY_SCREEN_THIRD0) //first third
        {
            if (hours == CLOCK_DISPLAY_HOURS_MIN_ROLL) //hours can't go to zero, bo back to top
            {
                hours = CLOCK_DISPLAY_HOURS_MAX_ROLL;
            }
            else //just dec normaly
                hours--;
        }
        else if (touchCursorX <= CLOCK_DISPLAY_SCREEN_THIRD2) //second third
        {
            if (minutes == CLOCK_DISPLAY_MINUTES_MIN_ROLL) //minutes can't go neg, to back to top
                minutes = CLOCK_DISPLAY_MINUTES_MAX_ROLL;
            else
                minutes--; //dec normally
        }
        else //third third
        {
            if(seconds == CLOCK_DISPLAY_SECONDS_MIN_ROLL) //seconds can't go neg
                seconds = CLOCK_DISPLAY_SECONDS_MAX_ROLL; //back to top
            else
                seconds--; //dec normally
        }
    }
    else // its on the top
    {
        //we're goin go to inc
        if (touchCursorX < CLOCK_DISPLAY_SCREEN_THIRD0) //first third
        {
            if (hours == CLOCK_DISPLAY_HOURS_MAX_ROLL) //can't go over 12
                hours = CLOCK_DISPLAY_HOURS_MIN_ROLL; //rollover
            else
                hours++; //inc normally
        }
        else if (touchCursorX < CLOCK_DISPLAY_SCREEN_THIRD2) //second third
        {
            if (minutes == CLOCK_DISPLAY_MINUTES_MAX_ROLL) //can't go over 59
                minutes = CLOCK_DISPLAY_MINUTES_MIN_ROLL; //rollover
            else
                minutes++; //inc normally
        }
        else //third third
        {
            if (seconds == CLOCK_DISPLAY_SECONDS_MAX_ROLL) //can't go over 59
                seconds = CLOCK_DISPLAY_SECONDS_MIN_ROLL; //rollover
            else
                seconds++; //inc normally SUCKA
        }
    }
    clockDisplay_draw();
}

void clockDisplay_advanceTimeOneSecond()  // Advances the time forward by 1 second.
{
    seconds++; //advance 1 seconds
    if (seconds == CLOCK_DISPLAY_MAX_SECONDS) //if seconds at 60, reset and advance minutes
    {
        seconds = CLOCK_DISPLAY_SECONDS_MIN_ROLL;
        minutes++; //move along minuntes
        if (minutes == CLOCK_DISPLAY_MAX_MINUTES) //if minutes at 60, reset and advance hours
        {
            minutes = CLOCK_DISPLAY_MINUTES_MIN_ROLL;
            hours++;
            if (hours == CLOCK_DISPLAY_MAX_HOURS) //if hours at 12, reset
                hours = CLOCK_DISPLAY_HOURS_MIN_ROLL;
        }
    }
    clockDisplay_draw();
}

void clockDisplay_runTest()               // Run a test of clock-display functions.
{
    clockDisplay_init();




    for (int i = 0; i < CLOCK_DISPLAY_LESS_LOOPS; i++)
    {
        //inc hours
        touchCursorX = CLOCK_DISPLAY_HOURS; //move the cursors
        touchCursorY = CLOCK_DISPLAY_INC;
        clockDisplay_performIncDec(); //inc/dec base don the cursor location
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(CLOCK_DISPLAY_DELAY_1SEC);
    }

    utils_msDelay(CLOCK_DISPLAY_RUN_TEST_DELAY);

    for (int i = 0; i < CLOCK_DISPLAY_LESS_LOOPS; i++)
    {
        //dec hours
        touchCursorX = CLOCK_DISPLAY_HOURS; //move the cursors
        touchCursorY = CLOCK_DISPLAY_DEC;
        clockDisplay_performIncDec(); //inc/dec base don the cursor location
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(CLOCK_DISPLAY_DELAY_1SEC);
    }

    utils_msDelay(CLOCK_DISPLAY_DELAY_1SEC);

    for (int i = 0; i < CLOCK_DISPLAY_MORE_LOOPS; i++)
    {
        //inc min
        touchCursorX = CLOCK_DISPLAY_MINUTES; //move the cursors
        touchCursorY = CLOCK_DISPLAY_INC;
        clockDisplay_performIncDec(); //inc/dec base don the cursor location
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(CLOCK_DISPLAY_RUN_TEST_DELAY);
    }

    utils_msDelay(CLOCK_DISPLAY_DELAY_1SEC);

    for (int i = 0; i < CLOCK_DISPLAY_MORE_LOOPS; i++)
    {
        //dec min
        touchCursorX = CLOCK_DISPLAY_MINUTES; //move the cursors
        touchCursorY = CLOCK_DISPLAY_DEC;
        clockDisplay_performIncDec(); //inc/dec base don the cursor location
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(CLOCK_DISPLAY_RUN_TEST_DELAY);
    }

    utils_msDelay(CLOCK_DISPLAY_DELAY_1SEC);

    for (int i = 0; i < CLOCK_DISPLAY_MORE_LOOPS; i++)
    {
        //inc secs
        touchCursorX = CLOCK_DISPLAY_SECONDS; //move the cursors
        touchCursorY = CLOCK_DISPLAY_INC;
        clockDisplay_performIncDec(); //inc/dec base don the cursor location
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(CLOCK_DISPLAY_RUN_TEST_DELAY);
    }

    utils_msDelay(CLOCK_DISPLAY_DELAY_1SEC);

    for (int i = 0; i < CLOCK_DISPLAY_MORE_LOOPS; i++)
    {
        //dec secs
        touchCursorX = CLOCK_DISPLAY_SECONDS; //move the cursors
        touchCursorY = CLOCK_DISPLAY_DEC;
        clockDisplay_performIncDec(); //inc/dec base don the cursor location
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(CLOCK_DISPLAY_RUN_TEST_DELAY);
    }

    while(1)
    {
        clockDisplay_advanceTimeOneSecond();
        clockDisplay_draw(); //draw redraw the timer
        utils_msDelay(10);
    }


}
