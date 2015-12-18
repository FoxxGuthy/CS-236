/*
 * wamDisplay.c
 *
 *  Created on: Nov 20, 2015
 *      Author: skguth10
 */
#include "supportFiles/display.h"
#include "supportFiles/utils.h" //for testing
#include "src/buttons/switches.h" //for testing
#include "wamDisplay.h"
#include "wamControl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> //for debugging
#include "src/ticTac/ticTacToeDisplay.h" //import this to use the ticTacToeDisplay_touchScreenComputerBoardrowColumn function


#define WAM_D_GAME_OVER_MESSAGE1 "Game Over\n\r"
#define WAM_D_GAME_OVER_MESSAGE2_FORMAT "    Hits: %d\n  Misses: %d\nFinal Level: %d\n\n(Touch to Try Again)\n\r"
#define WAM_D_BIG_TEXT 3 //for game over text
#define WAM_D_NORMAL_TEXT 2 //everything else
#define WAM_D_MESSAGE_OFFSET 4//how far to push down game over message
static char gameOverStatString[100]; //will hold stats in message2 format

static uint16_t oldHits = 0; //old hits is whats on the screen, we have this so we can draw it in black and replace with currentHits
static uint16_t oldMisses = 0;
static uint16_t currentLevel = 1;
#define MISS_SCORE_OFFSET 12 //how many chars over the miss score is display
#define LEVEL_SCORE_OFFSET 22 //how many chars over the level score is displayed

#define WAM_INSTRUCTIONS " Whack a Mole!\n Touch Screen To Start" //displayed at beginning
#define WAM_SCORE_SCREEN "Hit:0  Miss:0   Level:1" //the initial score strip

#define WAM_HOLE_SEPERATION_OFFSET_X 106 //distance between hole centers
#define WAM_HOLE_FROM_LEFT (WAM_HOLE_SEPERATION_OFFSET_X/2)// how far the hole is from left edge. 106/2

#define WAM_HOLE_SEPERATION_OFFSET_Y 74 //distance between hole centers in Y
#define WAM_HOLE_FROM_TOP (WAM_HOLE_SEPERATION_OFFSET_Y/2) //how far from top

#define WAM_HOLE_POSITION0 0
#define WAM_HOLE_POSITION1 1
#define WAM_HOLE_POSITION2 2
#define WAM_HOLE_COUNT 3

#define WAM_HOLE_RADIUS 30 //size of hole
#define WAM_MOLE_RADIUS 27 //size of mole
#define HITS_PER_LEVEL 5 //how often the level will increment based on number of hits

#define WAM_ERROR "ERRONOUS STATE" //for defaults in switches

static wamDisplay_moleCount_e currentMoleCount = wamDisplay_moleCount_9; //instance of the molecount, default 9

#define WAM_ARRAYSIZE_9 9 //for when assigning pointers, we'll need 9, 6, 4
#define WAM_ARRAYSIZE_6 6
#define WAM_ARRAYSIZE_4 4
static uint16_t currentBoardsize = 0; //will be set with 9,6,4 to for freeing memory later

static uint16_t activeMoleCount = 0; //how many moles active at once



/********************** typedefs **********************/
// This keeps track of all mole information.
typedef struct {
    wamDisplay_point_t origin;  // This is the origin of the hole for this mole.
    // A mole is active if either of the tick counts are non-zero. The mole is dormant otherwise.
    // During operation, non-zero tick counts are decremented at a regular rate by the control state machine.
    // The mole remains in his hole until ticksUntilAwake decrements to zero and then he pops out.
    // The mole remains popped out of his hole until ticksUntilDormant decrements to zero.
    // Once ticksUntilDomant goes to zero, the mole hides in his hole and remains dormant until activated again.
    wamDisplay_moleTickCount_t ticksUntilAwake;  // Mole will wake up (pop out of hole) when this goes from 1 -> 0.
    wamDisplay_moleTickCount_t ticksUntilDormant; // Mole will go dormant (back in hole) this goes 1 -> 0.
} wamDisplay_moleInfo_t;

// This will contain pointers to all of the mole info records.
// This will ultimately be treated as an array of pointers.
static wamDisplay_moleInfo_t** wamDisplay_moleInfo;

void wamDisplay_allocateMemory(uint16_t arraySize)
{
    //allocate memory to hold 9 pointers
    wamDisplay_moleInfo = (wamDisplay_moleInfo_t**) malloc(arraySize * sizeof(wamDisplay_moleInfo_t*));
    //    printf("address: 0x%x:\n\r ", wamDisplay_moleInfo);
    //allocate memory for each struct
    for (uint16_t i = 0; i < arraySize; i++) //iterate thru array of pointers
    {
        //point each one to a struct sized chunk of memeory
        wamDisplay_moleInfo[i] = (wamDisplay_moleInfo_t*) malloc(sizeof(wamDisplay_moleInfo_t));
    }

    uint16_t index = 0;
    switch(arraySize)
    {
    case WAM_ARRAYSIZE_9:
        for (uint16_t j = 0; j < WAM_HOLE_COUNT; j++)
        {
            for (uint16_t s = 0; s < WAM_HOLE_COUNT; s++)
            {
                wamDisplay_moleInfo[index]->origin.x = s; //give it ahome
                wamDisplay_moleInfo[index]->origin.y = j;
                wamDisplay_moleInfo[index]->ticksUntilAwake = 0; //intialize, don't leave floating
                wamDisplay_moleInfo[index]->ticksUntilDormant = 0;
                index++;
            }
        }
        break;
    case WAM_ARRAYSIZE_4:
        for (uint16_t j = 0; j < WAM_HOLE_COUNT; j++)
        {
            for (uint16_t s = 0; s < WAM_HOLE_COUNT; s++)
            {
                wamDisplay_moleInfo[index]->origin.x = j; //give it ahome
                wamDisplay_moleInfo[index]->origin.y = s;
                wamDisplay_moleInfo[index]->ticksUntilAwake = 0; //intialize, don't leave floating
                wamDisplay_moleInfo[index]->ticksUntilDormant = 0;
                s++; //skip middle column
                index++;
            }
            j++; //skip middle row
        }
        break;
    case WAM_ARRAYSIZE_6:
        for (uint16_t j = 0; j < WAM_HOLE_COUNT; j++)
        {
            for (uint16_t s = 0; s < WAM_HOLE_COUNT; s++)
            {
                wamDisplay_moleInfo[index]->origin.x = s; //give it ahome
                wamDisplay_moleInfo[index]->origin.y = j;
                wamDisplay_moleInfo[index]->ticksUntilAwake = 0; //intialize, don't leave floating
                wamDisplay_moleInfo[index]->ticksUntilDormant = 0;
                index++;
            }
            j++; //skip middle row
        }
        break;
    default:
        printf(WAM_ERROR);
        break;
    }
    //verify it worked
//    for (uint16_t k=0; k<arraySize; k++)
//    {
//        printf("Struct contents index:%d x:%d, y:%d\n\r", k, wamDisplay_moleInfo[k]->origin.x, wamDisplay_moleInfo[k]->origin.y);
//    }
}


void wamDisplay_freeMemory(uint16_t arraySize)
{
    for (uint16_t l=0; l<arraySize; l++)
    {
        free(wamDisplay_moleInfo[l]);   // This deallocates the memory.
        wamDisplay_moleInfo[l] = NULL;  // This step is not necessary but will keep you from reusing deallocated memory.
    }
    // Next, deallocate the array that contains the pointers to the structs.
    free(wamDisplay_moleInfo);   // Deallocates the container of arrays.
    wamDisplay_moleInfo = NULL;  // Also keeps you from reusing the deallocated memory.
}

// Allocates the memory for wamDisplay_moleInfo_t records.
// Computes the origin for each mole assuming a simple row-column layout:
// 9 moles: 3 rows, 3 columns, 6 moles: 2 rows, 3 columns, 4 moles: 2 rows, 2 columns
// Also inits the tick counts for awake and dormant.
void wamDisplay_computeMoleInfo() {
    // Setup all of the moles, creates and inits mole info records.
    // Create the container array. It contains pointers to each of the mole-hole info records.
    //    wamDisplay_moleInfo =  // Need to uncomment this line and initialize this variable via malloc().

    //set arrays here, give hole. use switch here
    switch(currentMoleCount)
    {
    case wamDisplay_moleCount_9:
        wamDisplay_allocateMemory(WAM_ARRAYSIZE_9); //allocate 9 pointers and structs
        currentBoardsize = WAM_ARRAYSIZE_9; //set board size so we can free this much later
        break;
    case wamDisplay_moleCount_6:
        wamDisplay_allocateMemory(WAM_ARRAYSIZE_6); //allocate 6 pointers and structs
        currentBoardsize = WAM_ARRAYSIZE_6;
        break;
    case wamDisplay_moleCount_4:
        wamDisplay_allocateMemory(WAM_ARRAYSIZE_4); //allocate 4 pointers and structs
        currentBoardsize = WAM_ARRAYSIZE_4;
        break; //dont want to hit default
    default:
        printf(WAM_ERROR);
        break;
    }
}

void wamDisplay_selectMoleCount(wamDisplay_moleCount_e moleCount)
{
    currentMoleCount = moleCount;
}

void wamDisplay_init()
{
    wamDisplay_computeMoleInfo(); //allocate the memory
    wamDisplay_drawScoreScreen(); //draw the score strip
}

// Draw the game display with a background and mole holes.
void wamDisplay_drawMoleBoard()
{
    display_fillRect(0,0,DISPLAY_WIDTH,  //draw a green square that doesn't cover the score strip
            DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT), DISPLAY_GREEN);
    switch (currentMoleCount)
    {
    case wamDisplay_moleCount_9:
        for (uint8_t i = 0; i < WAM_HOLE_COUNT; i++) //iterate over rows
        {
            for (uint8_t j = 0; j <WAM_HOLE_COUNT; j++) //iterate over coumns
            {
                display_fillCircle(WAM_HOLE_FROM_LEFT+WAM_HOLE_SEPERATION_OFFSET_X*i, //intial hole spot, with row seperation
                        WAM_HOLE_SEPERATION_OFFSET_Y*j+WAM_HOLE_FROM_TOP, //initial hole spot, with seperation
                        WAM_HOLE_RADIUS, DISPLAY_BLACK); //hole size, color
            }
        }

        break;
    case wamDisplay_moleCount_6:
        for (uint8_t i = 0; i < WAM_HOLE_COUNT; i++) //iterate over rows
        {
            for (uint8_t j = 0; j <WAM_HOLE_COUNT; j++) //iterate over coumns
            {
                display_fillCircle(WAM_HOLE_FROM_LEFT+WAM_HOLE_SEPERATION_OFFSET_X*i, //intial hole spot, with row seperation
                        WAM_HOLE_SEPERATION_OFFSET_Y*j+WAM_HOLE_FROM_TOP, //initial hole spot, with seperation
                        WAM_HOLE_RADIUS, DISPLAY_BLACK); //hole size, color
                j++; //get an extra j++ so we skip the middle row
            }
        }
        break;
    case wamDisplay_moleCount_4:
        for (uint8_t i = 0; i < WAM_HOLE_COUNT; i++) //iterate over rows
        {
            for (uint8_t j = 0; j <WAM_HOLE_COUNT; j++) //iterate over coumns
            {
                display_fillCircle(WAM_HOLE_FROM_LEFT+WAM_HOLE_SEPERATION_OFFSET_X*i, //intial hole spot, with row seperation
                        WAM_HOLE_SEPERATION_OFFSET_Y*j+WAM_HOLE_FROM_TOP, //initial hole spot, with seperation
                        WAM_HOLE_RADIUS, DISPLAY_BLACK); //hole size, color
                j++; //get an extra j++ so we skip the middle row
            }
            i++; //extra i++ to skip middle column
        }
        break;
    default:
        printf(WAM_ERROR);
        break;
    }
}

void wamDisplay_drawMole(wamDisplay_point_t* location) //pass in mole spot
{
    display_fillCircle(WAM_HOLE_FROM_LEFT+WAM_HOLE_SEPERATION_OFFSET_X * location->x, //intial hole spot, with row seperation
            WAM_HOLE_SEPERATION_OFFSET_Y*location->y+WAM_HOLE_FROM_TOP, //initial hole spot, with seperation
            WAM_MOLE_RADIUS, DISPLAY_BLUE); //hole size, color
}

void wamDisplay_eraseMole(wamDisplay_point_t* location)
{
    display_fillCircle(WAM_HOLE_FROM_LEFT+WAM_HOLE_SEPERATION_OFFSET_X * location->x, //intial hole spot, with row seperation
            WAM_HOLE_SEPERATION_OFFSET_Y*location->y+WAM_HOLE_FROM_TOP, //initial hole spot, with seperation
            WAM_MOLE_RADIUS, DISPLAY_BLACK); //hole size, color
}

// Draw the initial splash (instruction) screen.
void wamDisplay_drawSplashScreen()
{
    display_setTextSize(WAM_D_NORMAL_TEXT);
    display_fillScreen(DISPLAY_BLACK); //clear screen
    //position in center ish
    display_setCursor(WAM_D_MESSAGE_OFFSET*DISPLAY_CHAR_WIDTH * WAM_D_NORMAL_TEXT,
            WAM_D_NORMAL_TEXT*DISPLAY_CHAR_HEIGHT);
    //print
    display_println(WAM_INSTRUCTIONS);
}

// Draw the game-over screen.
void wamDisplay_drawGameOverScreen()
{
    wamDisplay_freeMemory(currentBoardsize); //deallocate the memory

    display_fillScreen(DISPLAY_BLACK); //clear screen
    //center and move down the Game Over message
    display_setCursor(WAM_D_MESSAGE_OFFSET*DISPLAY_CHAR_WIDTH * WAM_D_BIG_TEXT,
            WAM_D_MESSAGE_OFFSET*DISPLAY_CHAR_HEIGHT*WAM_D_BIG_TEXT);
    display_setTextSize(WAM_D_BIG_TEXT); //set size of text
    display_println(WAM_D_GAME_OVER_MESSAGE1); //print GAME OVER
    display_setTextSize(WAM_D_NORMAL_TEXT); //lower text size
    sprintf(gameOverStatString, WAM_D_GAME_OVER_MESSAGE2_FORMAT, oldHits, oldMisses, currentLevel);
    display_println(gameOverStatString); //display
    wamDisplay_resetAllScoresAndLevel(); //after printing, reset it all.
}

// Selects a random mole and activates it.
// Activating a mole means that the ticksUntilAwake and ticksUntilDormant counts are initialized.
// See the comments for wamDisplay_moleInfo_t for details.
// Returns true if a mole was successfully activated. False otherwise. You can
// use the return value for error checking as this function should always be successful
// unless you have a bug somewhere.
bool wamDisplay_activateRandomMole()
{
    bool unassigned = true;
    while(unassigned)
    {
        //check to see that timetillawake and time till dormant are both 0, then mole can be activated
        uint16_t randomMole = rand() % currentBoardsize; //generate number from 0 to (currentSize - 1)
        assert(randomMole < currentBoardsize); // just in case, if so we don't go out of range

        //if it has no ticks till dormant, and and none till awake, it it must not be activated
        if ((wamDisplay_moleInfo[randomMole]->ticksUntilDormant == 0)
                && (wamDisplay_moleInfo[randomMole]->ticksUntilAwake == 0))
        {
            unassigned = false; //we made assignment, stop looping
            randomSeed++; //inc seed
            wamDisplay_moleInfo[randomMole]->ticksUntilAwake = wamControl_getRandomMoleAsleepInterval();
            //this timer runs concurrently so its needs the time until awake added on
            activeMoleCount++; //one more active mole
        }
        else //its already active, pick another
        {
            randomSeed++; //inc the seed since the last one didn't work
            srand(randomSeed); //reseed avoid infinite loop
        }
    }
    return true;
}

// This takes the provided coordinates and attempts to whack a mole. If a
// mole is successfully whacked, all internal data structures are updated and
// the display and score is updated. You can only whack a mole if the mole is awake (visible).
wamDisplay_moleIndex_t wamDisplay_whackMole(wamDisplay_point_t* whackOrigin)
{
    bool hitMole = false;
    //so check if time until active is 0, and dormant is not zero at that particular
    for(uint16_t i = 0; i < currentBoardsize; i++) //go thru all the moles
    {
        //the got to the mole with that origin
        if ((wamDisplay_moleInfo[i]->origin.x == whackOrigin->x) &&  //check row is the same
                (wamDisplay_moleInfo[i]->origin.y == whackOrigin->y)) //check column
        {
            if (wamDisplay_moleInfo[i]->ticksUntilDormant != 0) //make sure its awake
            {
                wamDisplay_moleInfo[i]->ticksUntilDormant = 0; //reset
                wamDisplay_moleInfo[i]->ticksUntilAwake = 0; //reset timer. It is now not active
                wamDisplay_eraseMole(whackOrigin); //erase the mole
                wamDisplay_setHitScore(wamDisplay_getHitScore() + 1); //inc hits
                activeMoleCount--; //one less mole active
                if(oldHits % HITS_PER_LEVEL == 0) //if we're at something divisible by hits per level
                    wamDisplay_incrementLevel(); //inc level
                return i;
            }
            else //if its not active, its a not miss
            {
                //do nothing
                return i;
            }
        }
    }

}

// This updates the ticksUntilAwake/ticksUntilDormant clocks for all of the moles.
void wamDisplay_updateAllMoleTickCounts()
{
    for(uint16_t i = 0; i < currentBoardsize; i++) //go thru all the moles
    {
        if (wamDisplay_moleInfo[i]->ticksUntilAwake != 0)
        {
            wamDisplay_moleInfo[i]->ticksUntilAwake--; //decrement time till awake
            if(wamDisplay_moleInfo[i]->ticksUntilAwake == 0) //if we're at zero now, need to wake mole, draw it
            {
                //draw the mole. while moleinfo is a pointer, origin isn't, so pass by reference
                wamDisplay_drawMole(&wamDisplay_moleInfo[i]->origin);
                //set time till sleep
                wamDisplay_moleInfo[i]->ticksUntilDormant = wamControl_getRandomMoleAwakeInterval();
            }
        }
        else if (wamDisplay_moleInfo[i]->ticksUntilDormant != 0) //if activate, wait till next time to dec
        {
            wamDisplay_moleInfo[i]->ticksUntilDormant--; //dec time till sleep
            // if time is now zero, its dormant
            if(wamDisplay_moleInfo[i]->ticksUntilDormant == 0) //if we're at zero now, need to wake mole, draw it
            {
                wamDisplay_eraseMole(&wamDisplay_moleInfo[i]->origin); //erase the mole
                wamDisplay_setMissScore(wamDisplay_getMissScore() + 1); //inc misses
                activeMoleCount--; //it ticked down. one less active mole.
            }
        }
    }
}


// Returns the count of currently active moles.
uint16_t wamDisplay_getActiveMoleCount()
{
    return activeMoleCount;
}

//pass currentHits in as hits
// Sets the hit value in the score window.
void wamDisplay_setHitScore(uint16_t hits)
{
    //move cursor to correct spot
    //4 chars over (Hit:) and bottom - 1 char height
    display_setCursor(DISPLAY_CHAR_WIDTH*WAM_D_MESSAGE_OFFSET*WAM_D_NORMAL_TEXT,
            DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    //cover up current score in black
    display_setTextColor(DISPLAY_BLACK);
    display_println(oldHits);
    //redraw with new score in white
    display_setTextColor(DISPLAY_WHITE);
    //move cursor back
    display_setCursor(DISPLAY_CHAR_WIDTH*WAM_D_MESSAGE_OFFSET*WAM_D_NORMAL_TEXT,
            DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    display_println(hits);
    oldHits = hits; //set old to newHits.
}

// Gets the current hit value.
uint16_t wamDisplay_getHitScore()
{
    //depending on use, may need oldHits
    return oldHits;
}

// Sets the miss value in the score window.
void wamDisplay_setMissScore(uint16_t misses)
{
    //move cursors to correct spot
    //14 chars over (hit:%   Miss:) and 1 char up
    display_setCursor(DISPLAY_CHAR_WIDTH*MISS_SCORE_OFFSET*WAM_D_NORMAL_TEXT, DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    //cover up current score in black
    display_setTextColor(DISPLAY_BLACK);
    display_println(oldMisses);
    //redraw with new score in white
    display_setTextColor(DISPLAY_WHITE);
    //move cursor back
    display_setCursor(DISPLAY_CHAR_WIDTH*MISS_SCORE_OFFSET*WAM_D_NORMAL_TEXT, DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    display_println(misses);
    oldMisses = misses; //set old to newHits.
}

// Gets the miss value.
uint16_t wamDisplay_getMissScore()
{
    return oldMisses;
}

// Sets the level value on the score board.
void wamDisplay_incrementLevel()
{
    //also increment max actives here
    if ( wamControl_getMaxActiveMoles() < currentBoardsize) ///cant have more active moles than holes
        wamControl_setMaxActiveMoles(wamControl_getMaxActiveMoles() + 1);

    //move cursors to correct spot
    //14 chars over (hit:%   Miss:%   Level:) and 1 char up
    display_setCursor(DISPLAY_CHAR_WIDTH*LEVEL_SCORE_OFFSET*WAM_D_NORMAL_TEXT, DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    //cover up current score in black
    display_setTextColor(DISPLAY_BLACK);
    display_println(currentLevel);
    currentLevel++; //inclevel
    //redraw with new score in white
    display_setTextColor(DISPLAY_WHITE);
    //move cursor back
    display_setCursor(DISPLAY_CHAR_WIDTH*LEVEL_SCORE_OFFSET*WAM_D_NORMAL_TEXT, DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    display_println(currentLevel);
}

// Retrieves the current level value.
uint16_t wamDisplay_getLevel()
{
    return currentLevel;
}

// Completely draws the score screen.
void wamDisplay_drawScoreScreen()
{
    //the score screen is the number strip at the bottom of the screen
    //set the cursor to bottom row of screen
    display_setTextSize(WAM_D_NORMAL_TEXT);
    display_setTextColor(DISPLAY_WHITE);
    display_setCursor(0, DISPLAY_HEIGHT-(DISPLAY_CHAR_HEIGHT*WAM_D_NORMAL_TEXT));
    display_println(WAM_SCORE_SCREEN);
}

// Make this function available for testing purposes.
void wamDisplay_incrementMissScore()
{
    oldMisses++;
}

// Reset the scores and level to restart the game.
void wamDisplay_resetAllScoresAndLevel()
{
    oldHits = 0; //reset
    oldMisses = 0; //called "old" because it gets drawn overitself then is replaced
    currentLevel = 1;
    activeMoleCount = 0;
    wamControl_setMaxActiveMoles(1);
}

// Test function that can be called from main() to demonstrate milestone 1.
// Invoking this function should provide the same behavior as shown in the Milestone 1 video.
void wamDisplay_runMilestone1_test()
{
    wamDisplay_point_t steve;
    steve.x = 1;
    steve.y = 1;

    while(1)
    {
        wamDisplay_drawSplashScreen();
        while(!display_isTouched()); //wait for touch
        wamDisplay_drawMoleBoard();
        wamDisplay_drawScoreScreen();
        while(!display_isTouched());//wait for touch
        wamDisplay_drawMole(&steve);
        while(!display_isTouched());
        wamDisplay_eraseMole(&steve);
        while(!display_isTouched());
        wamDisplay_drawGameOverScreen(); //show score
        while(!display_isTouched()); //wait for touch
    }
}




