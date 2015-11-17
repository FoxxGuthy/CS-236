/*
 * flashSequence.c
 *
 *  Created on: Nov 5, 2015
 *      Author: skguth10
 */

#include "flashSequence.h"
#include "supportFiles/display.h"
#include "simonDisplay.h"
#include "buttonHandler.h"
#include "supportFiles/utils.h" // for testing purposes
#include "globals.h"

static bool flashSequenceEnabled = false; //will it run, controlled by flashSequence_enable/disable

//the states
enum FS_States {FS_init_st, FS_draw_st, FS_wait_to_clear_st, FS_clear_st, FS_complete_st} FS_State;

//also doubles as index for sequence
static uint16_t sequenceLengthCount = 0; //when we draw the sequence, will count how many times we're in draw/clear
static uint8_t FS_regionNumber = 0; //current region
static uint16_t waitTimer = 0; //timer for how long to hold the square until clearing
#define WAITTIMER_EXPIRE 8 //how long to show square. tryying to be 500m/s

void flashSequence_enable()
{
    flashSequenceEnabled = true; //turn on the state machine
}

void flashSequence_disable()
{
    flashSequenceEnabled = false; //disable the bool
    FS_State = FS_init_st; //return to init state for next time
}

bool flashSequence_completed()
{
    return (FS_State == FS_complete_st); //we're in completed state, so it must have completed. return true
}

void flashSequence_tick()
{
    if(flashSequenceEnabled) //if enabled, proceed
    {
        //ACTIONS////////////////////////////////////////////////////////////////////////////////////////
        switch(FS_State)
        {
        case FS_init_st:
            sequenceLengthCount = 0; //reset the count
            waitTimer = 0; //reset waitTImer
            break;
        case FS_draw_st:
            waitTimer = 0; //reset the timer again
            FS_regionNumber = globals_getSequenceValue(sequenceLengthCount); //get the region
            simonDisplay_drawSquare(FS_regionNumber, false); //draw the square
            break;
        case FS_wait_to_clear_st:
            waitTimer++; //inc the timer
            break;
        case FS_clear_st:
            sequenceLengthCount++; //inc the index
            //clear the square
            simonDisplay_drawSquare(FS_regionNumber, true);
            break;
        case FS_complete_st:

            break;
        }

        //TRANSITIONS////////////////////////////////////////////////////////////////////////////////////
        switch(FS_State)
        {
        case FS_init_st:
            FS_State = FS_draw_st; //just move along
            break;
        case FS_draw_st:
            FS_State = FS_wait_to_clear_st;
            break;
        case FS_wait_to_clear_st:
            if (waitTimer >= WAITTIMER_EXPIRE) //reached the timer limit
                FS_State = FS_clear_st; //ok to clear
            break;
        case FS_clear_st:
            if (sequenceLengthCount == globals_getSequenceIterationLength()) //if we're at the end
            {
                FS_State = FS_complete_st; //go to complete state
            }
            else //not at end
            {
                FS_State = FS_draw_st;
            }
            break;
        case FS_complete_st:
            //do nothing. FINAL STATE
            break;
        }
    }
}

// This will set the sequence to a simple sequential pattern.
// It starts by flashing the first color, and then increments the index and flashes the first
// two colors and so forth. Along the way it prints info messages to the LCD screen.
#define TEST_SEQUENCE_LENGTH 8  // Just use a short test sequence.
uint8_t flashSequence_testSequence[TEST_SEQUENCE_LENGTH] = {SIMON_DISPLAY_REGION_0,
        SIMON_DISPLAY_REGION_1,
        SIMON_DISPLAY_REGION_2,
        SIMON_DISPLAY_REGION_3,
        SIMON_DISPLAY_REGION_3,
        SIMON_DISPLAY_REGION_2,
        SIMON_DISPLAY_REGION_1,
        SIMON_DISPLAY_REGION_0};
#define INCREMENTING_SEQUENCE_MESSAGE1 "Incrementing Sequence"  // Info message.
#define RUN_TEST_COMPLETE_MESSAGE "Runtest() Complete"      // Info message.
#define MESSAGE_TEXT_SIZE 2                                 // Make the text easy to see.

// Print the incrementing sequence message.
void flashSequence_printIncrementingMessage() {
    display_fillScreen(DISPLAY_BLACK);// Otherwise, tell the user that you are incrementing the sequence.
    display_setCursor(0, display_height()/2);     // Roughly centered.
    display_println(INCREMENTING_SEQUENCE_MESSAGE1);  // Print the message.
    utils_msDelay(2000);                              // Hold on for 2 seconds.
    display_fillScreen(DISPLAY_BLACK);            // Clear the screen.
}

void flashSequence_runTest() {
    display_init();   // We are using the display.
    display_fillScreen(DISPLAY_BLACK);    // Clear the display.
    globals_setSequence(flashSequence_testSequence, TEST_SEQUENCE_LENGTH);    // Set the sequence.
    flashSequence_enable();                   // Enable the flashSequence state machine.
    int16_t sequenceLength = 1;                           // Start out with a sequence of length 1.
    globals_setSequenceIterationLength(sequenceLength);   // Set the iteration length.
    display_setTextSize(MESSAGE_TEXT_SIZE);           // Use a standard text size.
    while (1) {                   // Run forever unless you break.
        flashSequence_tick();   // tick the state machine.
        utils_msDelay(1);   // Provide a 1 ms delay.
        if (flashSequence_completed()) {  // When you are done flashing the sequence.
            flashSequence_disable();  // Interlock by first disabling the state machine.
            flashSequence_tick(); // tick is necessary to advance the state.
            utils_msDelay(1);     // don't really need this here, just for completeness.
            flashSequence_enable();   // Finish the interlock by enabling the state machine.
            utils_msDelay(1); // Wait 1 ms for no good reason.
            sequenceLength++; // Increment the length of the sequence.
            if (sequenceLength > TEST_SEQUENCE_LENGTH) // Stop if you have done the full sequence.
                break;
            flashSequence_printIncrementingMessage();  // Tell the user that you are going to the next step in the pattern.
            globals_setSequenceIterationLength(sequenceLength);   // Set the length of the pattern.
        }
    }
    // Let the user know that you are finished.
    display_fillScreen(DISPLAY_BLACK);
    display_setCursor(0, display_height()/2);
    display_println(RUN_TEST_COMPLETE_MESSAGE);
}
