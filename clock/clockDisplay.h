/*SETH GUTHRIE ECEN 330
 * clockDisplay.h
 *
 *  Created on: Sep 28, 2015
 *      Author: skguth10
 */

#include <stdbool.h>
#include "supportFiles/display.h"
#include "supportFiles/utils.h" // for testing purposes
#include <stdio.h>


#define CLOCK_DISPLAY_SCREEN_HEIGHT_MIDDLE (240/2) // these are the middle of the boards
#define CLOCK_DISPLAY_SCREEN_WIDTH_MIDDLE (320/2)

//text char width is 6 * the size
//text char height is 8 * the height
//so we'll set the chars to be have the 1/2 height (of the char) past the middle of the board height
//we'll set the chars to be a full width left and right of width middle
#define CLOCK_DISPLAY_NUMBERS_HEIGHT (CLOCK_DISPLAY_SCREEN_HEIGHT_MIDDLE - (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_HEIGHT/2))
#define CLOCK_DISPLAY_TEXT_PIXEL_WIDTH (CLOCK_DISPLAY_TEXT_SIZE * DISPLAY_CHAR_WIDTH)


void clockDisplay_init();  // Called only once - performs any necessary inits.

void clockDisplay_updateTimeDisplay(bool forceUpdateAll);  // Updates the time display with latest time.

void clockDisplay_performIncDec();         // Performs the increment or decrement, depending upon the touched region.

void clockDisplay_advanceTimeOneSecond();  // Advances the time forward by 1 second.

void clockDisplay_runTest();               // Run a test of clock-display functions.
