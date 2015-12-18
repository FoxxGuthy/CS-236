/*
 * wamControl.c
 *
 *  Created on: Dec 2, 2015
 *      Author: skguth10
 */

#include "wamControl.h"
#include <stdio.h>
#include "supportFiles/display.h"

static int16_t WAMC_touchCursorX = 0; //where's the cursor
static int16_t WAMC_touchCursorY = 0;
static uint8_t WAMC_touchPressure = 0; //I don't think i'll actually use this, but I think get

#define WAMC_ROW_0 0 //these don't need to be defined, but since there is a row2 and column 2 we'll add to be consistent
#define WAMC_ROW_1 1
#define WAMC_ROW_2 2

#define WAMC_COLUMN_0 0 //#define for consistency
#define WAMC_COLUMN_1 1
#define WAMC_COLUMN_2 2

#define WAMC_VERTICAL_LINE_0 (DISPLAY_WIDTH/3)//seperates column 0 and 1
#define WAMC_VERTICAL_LINE_1 ((DISPLAY_WIDTH*2)/3)//seperates column 1 and 2

#define WAMC_HORIZONTAL_LINE_0 (DISPLAY_HEIGHT/3)//seperates row 0 and 1
#define WAMC_HORIZONTAL_LINE_1 ((DISPLAY_HEIGHT*2)/3)//seperates row 1 and 2

static uint16_t maxMissCount = 0; //once misses hits this, game is over
#define WAM_STATE_ERROR "INVALID STATE\n\r"

static uint16_t wamC_msPerTick = 0;

static uint16_t wamControl_settleTimer = 0;
#define SETTLE_TIMER_EXPIRE 1//needs to be 50m/s. period is 50 ms so just 1 tick

static uint8_t touchRow = 0;
static uint8_t touchColumn = 0;

static uint16_t maxActiveMoles = 0;

#define RAND_MOD 21 //for use in random interval generation
#define RAND_MOD_OFFSET 10//we don't want to have just 0, so we add

// Call this before using any wamControl_ functions.
void wamControl_init()
{
    //worthless
}

// Call this to set how much time is consumed by each tick of the controlling state machine.
// This information makes it possible to set the awake and sleep time of moles in ms, not ticks.
void wamControl_setMsPerTick(uint16_t msPerTick)
{
    wamC_msPerTick = msPerTick;
}

// This returns the time consumed by each tick of the controlling state machine.
uint16_t wamControl_getMsPerTick()
{
    return wamC_msPerTick;
}

//the states and initilize the current state to the init
enum WAM_States {WAM_init_st, WAM_update_mole_tick_st, WAM_activate_mole_st,
    WAM_settle_st, WAM_whack_st,} WAM_State = WAM_init_st;

    //looks at the touch data, and computes which row its in.
    void wamControl_touchScreenComputeBoardRowColumn(uint8_t* row, uint8_t* column)
    {
        display_clearOldTouchData();
        display_getTouchedPoint(&WAMC_touchCursorX, &WAMC_touchCursorY, &WAMC_touchPressure); //need to relay this info to the Cursors

        //------------------------------------------------------------
        //COMPUTE COLUMN
        if (WAMC_touchCursorX < WAMC_VERTICAL_LINE_0) //IS IT IN COLUMN 0
        {
            *column = WAMC_COLUMN_0;
        }

        else if(WAMC_touchCursorX <= WAMC_VERTICAL_LINE_1) //is it in COLUMN 1
        {
            *column = WAMC_COLUMN_1;
        }

        else if(WAMC_touchCursorX >= WAMC_VERTICAL_LINE_1) //is it in COLUMN 2
        {
            *column = WAMC_COLUMN_2;
        }
        //------------------------------------------------------------
        //COMPUTE ROW
        if (WAMC_touchCursorY < WAMC_HORIZONTAL_LINE_0) //IS IT IN ROW 0
        {
            *row = WAMC_ROW_0;
        }

        else if(WAMC_touchCursorY <= WAMC_HORIZONTAL_LINE_1) //is it in ROW 1
        {
            *row = WAMC_ROW_1;
        }

        else if(WAMC_touchCursorY >= WAMC_HORIZONTAL_LINE_1) //is it in ROW 2
        {
            *row = WAMC_ROW_2;
        }
    }

    void WAM_debugStatePrint() {
        static WAM_States previousState;
        static bool firstPass = true;
        // Only print the message if:
        // 1. This the first pass and the value for previousState is unknown.
        // 2. previousState != currentState - this prevents reprinting the same state name over and over.
        if (previousState != WAM_State || firstPass) {
            firstPass = false;                // previousState will be defined, firstPass is false.
            previousState = WAM_State;     // keep track of the last state that you were in.

            switch(WAM_State)
            {
            case WAM_init_st:
                printf("WAM_init_st\n\r");
                break;
            case WAM_update_mole_tick_st:
                printf("WAM_update_mole_tick_st\n\r");
                break;
            case WAM_activate_mole_st:
                printf("WAM_activate_mole_st\n\r");
                break;
            case WAM_settle_st:
                printf("WAM_settle_st\n\r");
                break;
            case WAM_whack_st:
                printf("WAM_whack_st\n\r");
                break;
            default:
                printf(WAM_STATE_ERROR);
                break;
            }
        }
    }

    // Standard tick function.
    void wamControl_tick()
    {
        WAM_debugStatePrint(); //print out current state for debugging

        //ACTIONS ******************************************************
        switch(WAM_State)
        {
        case WAM_init_st:
            //do nothing
            break;
        case WAM_update_mole_tick_st:
            wamDisplay_updateAllMoleTickCounts();
            break;
        case WAM_activate_mole_st:
            wamDisplay_activateRandomMole();
            break;
        case WAM_settle_st:
            wamControl_settleTimer++; //increment the timer
            break;
        case WAM_whack_st:
            //compue touch point the rows and columns
            wamControl_touchScreenComputeBoardRowColumn(&touchRow, &touchColumn);
            //make a point, and assign it values
            wamDisplay_point_t whackSpot;
            whackSpot.x = touchColumn;
            whackSpot.y = touchRow;
            wamDisplay_whackMole(&whackSpot); //check for hit scuess
            break;
        }

        //TRANSITIONS **************************************************

        switch(WAM_State)
        {
        case WAM_init_st:
            WAM_State = WAM_activate_mole_st; //always go here first
            break;
        case WAM_update_mole_tick_st:
            if (wamDisplay_getActiveMoleCount() < wamControl_getMaxActiveMoles()) //if not max moles activated
            {
                WAM_State = WAM_activate_mole_st;
            }
            if (display_isTouched()) //touch has priority, so have it last
            {
                WAM_State = WAM_settle_st;
            }
            break;
        case WAM_activate_mole_st:
            WAM_State = WAM_update_mole_tick_st;
            break;
        case WAM_settle_st:
            if (wamControl_settleTimer >= SETTLE_TIMER_EXPIRE)
            {
                WAM_State = WAM_whack_st;
                wamControl_settleTimer = 0; //reset the timer
            }
            break;
        case WAM_whack_st:
            WAM_State = WAM_update_mole_tick_st;
            break;
        default:
            printf(WAM_STATE_ERROR);
            break;
        }
    }

    void wamControl_setRandomSeed(uint32_t seed)
    {
        srand(seed);
    }

    // Returns a random value that indicates how long the mole should sleep before awaking.
    wamDisplay_moleTickCount_t wamControl_getRandomMoleAsleepInterval()
    {
        wamControl_setRandomSeed(randomSeed);
        //use wamControl_getMsPerTick() in here
        wamDisplay_moleTickCount_t interval = ((rand() % RAND_MOD) + RAND_MOD_OFFSET) / wamDisplay_getLevel() + 1;
        randomSeed++;
        return interval;
    }

    // Returns a random value that indicates how long the mole should stay awake before going dormant.
    wamDisplay_moleTickCount_t wamControl_getRandomMoleAwakeInterval()
    {
        wamControl_setRandomSeed(randomSeed);
        //use wamControl_getMsPerTick() in here
        wamDisplay_moleTickCount_t interval = (rand() % RAND_MOD) + RAND_MOD_OFFSET / wamDisplay_getLevel() + 1;
        randomSeed++;
        return interval;
    }

    // Set the maximum number of active moles.
    void wamControl_setMaxActiveMoles(uint16_t count)
    {
            maxActiveMoles = count;
    }

    // Get the current allowable count of active moles.
    uint16_t wamControl_getMaxActiveMoles()
    {
        return maxActiveMoles;
    }

    // Set the maximum number of misses until the game is over.
    void wamControl_setMaxMissCount(uint16_t missCount)
    {
        maxMissCount = missCount; //reassign the max misses
    }

    // Use this predicate to see if the game is finished.
    bool wamControl_isGameOver()
    {
        if (wamDisplay_getMissScore() >= maxMissCount) //misscount is at our set limit
        {
            return true; //game is over
        }
        else
            return false; //game not over
    }



