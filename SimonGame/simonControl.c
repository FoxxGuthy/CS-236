/*
 * simonControl.c
 *
 *  Created on: Nov 9, 2015
 *      Author: skguth10
 */

#include "simonControl.h"
#include "verifySequence.h"
#include "flashSequence.h"
#include "supportFiles/display.h"
#include "simonDisplay.h"
#include "buttonHandler.h"
#include "supportFiles/utils.h" // for testing purposes
#include "globals.h"
#include <stdlib.h>

#define REGION_COUNT 4 // how many regions. used with rand() % to get randoms from 0-3
static uint16_t sequenceSeed = 247; //used to seed random number

static uint16_t SC_sequence_length = 4; //used to set global current Sequence Length
static uint16_t SC_currentIndex = 1; //for flash/verify. location in sequence. also used as currentlength, so must be > 0
static uint16_t SC_maxScore = 0; //longest succesful sequence.
static uint16_t SC_currentScore = 0;
static char maxScoreString[40]; //a string for printing out max score
static bool printFlag = true; //use so the new level stuff doens't get printed every tick

#define SC_ERROR_EXPIRE 20 //how many cycles to show error
#define ERROR_MESSAGE_TIMEOUT "GAME OVER:\n NOT QUICK ENOUGH" //message
#define ERROR_MESSAGE_INPUT "GAME OVER:\n INCORRECT INPUT"
static uint16_t errorTimer = 0;

#define SC_INSTRUCTIONS "SIMON \n Touch To Start" //for instruction state

#define SC_INSTRUCTION_TEXTSIZE 3 //instructions bit bigger
#define SC_INSTRUCTION_CURSORX 100
#define SC_INSTRUCTION_CURSORY 50
#define SC_TEXTSIZE 2 //default text size
#define SC_LVL_MESSAGE "Level Complete \n Touch for new level"//shown when lvl is complete

#define SC_LVL_MESSAGE_EXPIRE 100 //give the users 10 seconds to start new lvl
static uint16_t lvlMessageTimer = 0;

#define SC_HIGHSCORE_MESSAGE "Longest sequence: %d" //for displaying high schore

enum SC_States {SC_init_st, SC_instructions_st, SC_get_sequence_st, SC_wait_first_touch_st, SC_flash_sequence_st,
    SC_verify_sequence_st, SC_error_st, SC_print_score_st, SC_level_complete_st} SC_State = SC_init_st;

    void SC_debugStatePrint() {
        static SC_States previousState;
        static bool firstPass = true;
        // Only print the message if:
        // 1. This the first pass and the value for previousState is unknown.
        // 2. previousState != currentState - this prevents reprinting the same state name over and over.
        if (previousState != SC_State || firstPass) {
            firstPass = false;                // previousState will be defined, firstPass is false.
            previousState = SC_State;     // keep track of the last state that you were in.

            switch(SC_State)
            {
            case SC_init_st:
                printf("SC_init_st\n\r");
                break;
            case SC_instructions_st:
                printf("SC_instructions_st\n\r");
                break;
            case SC_wait_first_touch_st:
                printf("SC_wait_first_touch_st\n\r");
                break;
            case SC_get_sequence_st:
                printf("SC_get_sequence_st\n\r");
                break;
            case SC_flash_sequence_st:
                printf("SC_flash_sequence_st\n\r");
                break;
            case SC_verify_sequence_st:
                printf("SC_verify_sequence_st\n\r");
                break;
            case SC_error_st:
                printf("SC_error_st\n\r");
                break;
            case SC_print_score_st:
                printf("SC_print_score_st\n\r");
                break;
            case SC_level_complete_st:
                printf("SC_level_complete_st\n\r");
                break;
            default:
                break;
            }
        }
    }

    void SC_setSequence()
    {
        srand(sequenceSeed); //seed the random number
        uint8_t randomSequence[GLOBALS_MAX_FLASH_SEQUENCE];
        for (uint16_t i = 0; i < SC_sequence_length; i++)
        {
            //something with random
            randomSequence[i] = rand() % REGION_COUNT;

        }
        globals_setSequence(randomSequence, SC_sequence_length);
        globals_setSequenceIterationLength(SC_currentIndex);
    }

    void simonControl_tick()
    {
        SC_debugStatePrint();


        /// //ACTIONS***********************************************************************
        switch(SC_State)
        {
        case SC_init_st:
            //do nothing
            break;
        case SC_instructions_st:
            display_setTextColor(DISPLAY_WHITE);
            display_setCursor(SC_INSTRUCTION_CURSORX,SC_INSTRUCTION_CURSORY);
            display_setTextSize(SC_INSTRUCTION_TEXTSIZE);
            display_println(SC_INSTRUCTIONS);
            break;
        case SC_wait_first_touch_st:
            // do nothing
            break;
        case SC_get_sequence_st:
            SC_currentScore = 0; //reset
            SC_setSequence();
            break;
        case SC_flash_sequence_st:
            //do nothing
            break;
        case SC_verify_sequence_st:
            //do nothing
            break;
        case SC_error_st:
            errorTimer++;
            if (verifySequence_isTimeOutError())
            {
                display_setTextSize(SC_TEXTSIZE);
                display_println(ERROR_MESSAGE_TIMEOUT);
            }
            else if (verifySequence_isUserInputError())
            {
                display_setTextSize(SC_TEXTSIZE);
                display_println(ERROR_MESSAGE_INPUT);
            }
            verifySequence_disable();
            break;
        case SC_print_score_st:
            sprintf(maxScoreString, SC_HIGHSCORE_MESSAGE, SC_maxScore);
            display_setTextSize(SC_TEXTSIZE); //decrease text size
            display_println(maxScoreString);
            //reset scores, index, and sequence
            SC_currentIndex = 1; //reset to 1. since this is sequence length, when we setSequence, don't put to zero
            SC_maxScore = 0; //reset
            errorTimer = 0; //reset timer
            lvlMessageTimer = 0;
            break;
        case SC_level_complete_st:
            lvlMessageTimer++;
            if(printFlag)
            {
                display_println(SC_LVL_MESSAGE);
                printFlag = false;
            }
            break;
        default:
            break;
        }


        //TRANSITIONS***********************************************************************
        switch(SC_State)
        {
        case SC_init_st:
            SC_State = SC_instructions_st; //just go to instructions
            break;
        case SC_instructions_st:
            SC_State = SC_wait_first_touch_st; //go wait after drawing.
            break;
        case SC_wait_first_touch_st:
            if (display_isTouched()) //if touched, move ahead
            {
                SC_State = SC_get_sequence_st;
                display_fillScreen(DISPLAY_BLACK); //clear screen of instructions
            }
            break;
        case SC_get_sequence_st:
            SC_State = SC_flash_sequence_st;
            flashSequence_enable();
            break;
        case SC_flash_sequence_st:
            if (flashSequence_completed())
            {
                SC_State = SC_verify_sequence_st; //go to verify
                flashSequence_disable(); //disable flash machine
                simonDisplay_drawAllButtons(); //draw the buttons upon exit
                verifySequence_enable();
            }
            break;
        case SC_verify_sequence_st:
            if (verifySequence_isComplete()) //done verifying
            {
                //if there a time out error or wrong input error
                if (verifySequence_isTimeOutError() || verifySequence_isUserInputError()) //no errors
                {
                    SC_State = SC_error_st;
                    display_fillScreen(DISPLAY_BLACK); //clear buttons off screen
                }
                //level complete
                else if (globals_getSequenceIterationLength() == SC_sequence_length)
                {
                    SC_sequence_length++;
                    SC_State = SC_level_complete_st;
                    verifySequence_disable();
                    display_fillScreen(DISPLAY_BLACK); //clear buttons off screen
                    SC_currentIndex = 1; //reset
                    SC_maxScore = SC_currentScore;
                }
                else
                {
                    SC_State = SC_flash_sequence_st;
                    flashSequence_enable();
                    SC_currentIndex++; //how many to flash?
                    globals_setSequenceIterationLength(SC_currentIndex); //udate how many to falsh
                    verifySequence_disable();
                    display_fillScreen(DISPLAY_BLACK); //clear buttons off screen
                    SC_currentScore++;
                    if (SC_currentScore > SC_maxScore) //before first level is complete, maxScore doesn't get set
                    {
                        SC_maxScore = SC_currentScore; //this mostly before first level is complete
                    }
                }
            }
            break;
        case SC_error_st:
            if (errorTimer >= SC_ERROR_EXPIRE)
            {
                SC_State = SC_print_score_st; //print highest score
                display_fillScreen(DISPLAY_BLACK);
            }

            break;
        case SC_print_score_st:
            SC_State = SC_instructions_st;
            SC_maxScore = 0; //reset the maxScore!
            sequenceSeed++; //change the seed
            break;
        case SC_level_complete_st:
            if (lvlMessageTimer > SC_LVL_MESSAGE_EXPIRE) //timed out
            {
                SC_State = SC_print_score_st;
            }
            else if (display_isTouched())
            {
                SC_State = SC_instructions_st;
                display_fillScreen(DISPLAY_BLACK); // clear screen
                sequenceSeed++; //change the seed
                printFlag = true; //rest print flag
            }
            break;
        default:
            break;
        }

    }

