/*
 * buttonHandler.c
 *
 *  Created on: Oct 31, 2015
 *      Author: skguth10
 */

#include "buttonHandler.h"

#define RUN_TEST_TERMINATION_MESSAGE1 "buttonHandler_runTest()"
#define RUN_TEST_TERMINATION_MESSAGE2 "terminated."
#define RUN_TEST_TEXT_SIZE 2

#define CLOCK_CONTROL_TIMER_PERIOD 50.0E-3

//SBH short for ButtonHandler.
//The States
enum BH_States {BH_init_st, BH_wait_touch, BH_touch_settle, BH_draw, BH_clear, BH_released} BH_State;

#define BH_ADTIMER_EXPIRE 20//(1/CLOCK_CONTROL_TIMER_PERIOD/20) //the number of ticks required to allow the adTimer to reach 50ms
static uint8_t BH_adTimer = 0; //counts up to 50ms.

//will the state machine run, controlled by
//enable/disable functions
static bool buttonHandlerEnabled = false;

static int16_t BH_touchCursorX = 0; //where's your cursor boy? //used by getRegionNumber
static int16_t BH_touchCursorY = 0;
static uint8_t BH_touchPressure = 0; //not actually used

static uint8_t BH_regionNumber = 0; //whats the region boy

bool wasReleasedFlag = false; //for use with detecting release

uint8_t buttonHandler_getRegionNumber()
{
    display_clearOldTouchData(); //do I need this here? for release?
    display_getTouchedPoint(&BH_touchCursorX, &BH_touchCursorY, &BH_touchPressure); //get the touch point
//    printf(" X: %i\t",  BH_touchCursorX); for use with debugging
//    printf("Y: %i \n\r", BH_touchCursorY);

    BH_regionNumber = simonDisplay_computeRegionNumber(BH_touchCursorX, BH_touchCursorY); //set the region
    return BH_regionNumber;
}


void buttonHandler_enable()
{
    buttonHandlerEnabled = true; //switch the machine on
}


void buttonHandler_disable()
{
    buttonHandlerEnabled = false; //switch the machine off
    //return to init_state
    BH_State = BH_init_st;
}

bool buttonHandler_releaseDetected()
{
    return wasReleasedFlag;
}

void BH_debugStatePrint() {
    static BH_States previousState;
    static bool firstPass = true;
    // Only print the message if:
    // 1. This the first pass and the value for previousState is unknown.
    // 2. previousState != currentState - this prevents reprinting the same state name over and over.
    if (previousState != BH_State || firstPass) {
        firstPass = false;                // previousState will be defined, firstPass is false.
        previousState = BH_State;     // keep track of the last state that you were in.

        switch(BH_State)
        {
        case BH_init_st:
            printf("BH_init_st\n\r");
            break;
        case BH_wait_touch:
            printf("BH_wait_touch\n\r");
            break;
        case BH_touch_settle:
            printf("BH_touch_settle\n\r");
            break;
        case BH_draw:
            printf("BH_draw\n\r");
            break;
        case BH_clear:
            printf("BH_clear\n\r");
            break;
        }
    }
}

void buttonHandler_tick()
{
    //if enabled
    if(buttonHandlerEnabled)
    {
        BH_debugStatePrint();//debugging, prints current State

        //ACTION STATE MACHINE
        switch(BH_State)
        {
        case BH_init_st:
            BH_adTimer = 0; //reset the touch timer
            BH_regionNumber = 0; //reset region
            wasReleasedFlag = false; //resetflag
            //the main will draw the buttons, so we need not do so here
            break;
        case BH_wait_touch:
            //nothing. just wait
            break;
        case BH_touch_settle:
            BH_adTimer++; //count up;
            break;
        case BH_draw:
            //compute the region touched
            buttonHandler_getRegionNumber();
            //draw the square
            simonDisplay_drawSquare(BH_regionNumber, false);
            break;
        case BH_clear:
            //clear the square
            simonDisplay_drawSquare(BH_regionNumber, true);
            //redraw the button
            simonDisplay_drawButton(BH_regionNumber);
            wasReleasedFlag = true; //set flag
            break;
        }

        //TRANSITION STATE MACHINE
        switch(BH_State)
        {
        case BH_init_st:
            BH_State = BH_wait_touch;
            break;
        case BH_wait_touch:
            if (display_isTouched())
            {
                BH_State = BH_touch_settle;
            }
            break;
        case BH_touch_settle:
            if(BH_adTimer >= BH_ADTIMER_EXPIRE) //we reached the adTimer limit. screen has settle
            {
                BH_State = BH_draw;
            }
            break;
        case BH_draw:
            if (!display_isTouched()) //no longer touching screen
            {
                BH_State = BH_clear; //return to wait
            }
            break;
        case BH_clear:
            BH_State = BH_wait_touch;
        }
    }
}

// buttonHandler_runTest(int16_t touchCount) runs the test until
// the user has touched the screen touchCount times. It indicates
// that a button was pushed by drawing a large square while
// the button is pressed and then erasing the large square and
// redrawing the button when the user releases their touch.

void buttonHandler_runTest(int16_t touchCountArg) {
    int16_t touchCount = 0;             // Keep track of the number of touches.
    display_init();                     // Always have to init the display.
    display_fillScreen(DISPLAY_BLACK);  // Clear the display.
    simonDisplay_drawAllButtons();      // Draw the four buttons.
    buttonHandler_enable();
    while (touchCount < touchCountArg) {  // Loop here while touchCount is less than the touchCountArg
        buttonHandler_tick();               // Advance the state machine.
        utils_msDelay(1);           // Wait here for 1 ms.
        if (buttonHandler_releaseDetected()) {  // If a release is detected, then the screen was touched.
            touchCount++;                         // Keep track of the number of touches.
            printf("button released: %d\n\r", buttonHandler_getRegionNumber());  // Get the region number that was touched.
            buttonHandler_disable();             // Interlocked behavior: handshake with the button handler (now disabled).
            utils_msDelay(1);                     // wait 1 ms.
            buttonHandler_tick();                 // Advance the state machine.
            buttonHandler_enable();               // Interlocked behavior: enable the buttonHandler.
            utils_msDelay(1);                     // wait 1 ms.
            buttonHandler_tick();                 // Advance the state machine.
        }
    }
    display_fillScreen(DISPLAY_BLACK);            // clear the screen.
    display_setTextSize(RUN_TEST_TEXT_SIZE);      // Set the text size.
    display_setCursor(0, display_height()/2);     // Move the cursor to a rough center point.
    display_println(RUN_TEST_TERMINATION_MESSAGE1);   // Print the termination message on two lines.
    display_println(RUN_TEST_TERMINATION_MESSAGE2);
}




