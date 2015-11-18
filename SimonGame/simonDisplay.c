/*
 * simonDisplay.c
 *
 *  Created on: Oct 31, 2015
 *      Author: skguth10
 */


#include "SimonDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h" // for testing purposes
#include <stdio.h>

#define INVALID_REGION_ERROR "INVALID REGION\n\r"

#define TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS 1 // in ms
#define MAX_STR 255
#define TEXT_SIZE 2
// Runs a brief demonstration of how buttons can be pressed and squares lit up to implement the user
// interface of the Simon game. The routine will continue to run until the touchCount has been reached, e.g.,
// the user has touched the pad touchCount times.


//for drawButton()
#define BTN_X_POSITION 50 //this far from left edge, same amount on other side of BTN
#define BTN_Y_POSITION 30 //this far from top edge
#define BTN_X_OFFSET 160 //half screen over
#define BTN_Y_OFFSET 120 //half screen down

//for drawSqure()
#define SQUARE_WIDTH 160 //"squre" this wide
#define SQUARE_HEIGHT 120 //"height" this tall
#define SQUARE_X_OFFSET 160
#define SQUARE_Y_OFFSET 120

#define DISPLAY_RIGHT_SIDE 160
#define DISPLAY_BOTTOM_SIDE 120

int8_t simonDisplay_computeRegionNumber(int16_t x, int16_t y)
{
    if (x > DISPLAY_RIGHT_SIDE) //right side of screen
    {
        if(y > DISPLAY_BOTTOM_SIDE) //lower half of screen
        {
            return SIMON_DISPLAY_REGION_3; //bottom right = 3
        }
        else //top half of screen
        {
            return SIMON_DISPLAY_REGION_1; //top right = 1
        }
    }
    else //left side of screen
    {
        if(y > DISPLAY_BOTTOM_SIDE) //lower half of screen
        {
            return SIMON_DISPLAY_REGION_2; //bottom left = 2
        }
        else //top half of screen
        {
            return SIMON_DISPLAY_REGION_0; //top left = 0
        }
    }
    return 0; //better not arrive here. ERROR
}

void simonDisplay_drawButton(uint8_t regionNumber)
{
    switch(regionNumber)
    {
    case SIMON_DISPLAY_REGION_0: //red
        display_fillRect(BTN_X_POSITION,BTN_Y_POSITION,SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_RED);
        break;
    case SIMON_DISPLAY_REGION_1: //yellow
        display_fillRect(BTN_X_POSITION + BTN_X_OFFSET, BTN_Y_POSITION,SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_YELLOW);
        break;
    case SIMON_DISPLAY_REGION_2: //blue
        display_fillRect(BTN_X_POSITION, BTN_Y_POSITION + BTN_Y_OFFSET, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_BLUE);
        break;
    case SIMON_DISPLAY_REGION_3: //green
        display_fillRect(BTN_X_POSITION + BTN_X_OFFSET, BTN_Y_POSITION + BTN_Y_OFFSET, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_GREEN);
        break;
    default:
        printf(INVALID_REGION_ERROR);
        break;
    }
}

void simonDisplay_drawAllButtons()
{
    //rather than call thedrawButton 4 times, have the computation, we'll just draw 4 times
    //red - region0
    display_fillRect(BTN_X_POSITION,BTN_Y_POSITION,SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_RED);
    //yellow - region1
    display_fillRect(BTN_X_POSITION + BTN_X_OFFSET, BTN_Y_POSITION,SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_YELLOW);
    //blue - region2
    display_fillRect(BTN_X_POSITION, BTN_Y_POSITION + BTN_Y_OFFSET, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_BLUE);
    //green - region3
    display_fillRect(BTN_X_POSITION + BTN_X_OFFSET, BTN_Y_POSITION + BTN_Y_OFFSET, SIMON_DISPLAY_BUTTON_WIDTH, SIMON_DISPLAY_BUTTON_HEIGHT, DISPLAY_GREEN);

}

void simonDisplay_drawSquare(uint8_t regionNo, bool erase)
{
    switch(regionNo)
        {
        case SIMON_DISPLAY_REGION_0: //red, start at corner
            display_fillRect(0,0,SQUARE_WIDTH,SQUARE_HEIGHT, DISPLAY_RED);

            if(erase)
                display_fillRect(0,0,SQUARE_WIDTH,SQUARE_HEIGHT, DISPLAY_BLACK);//erase is true, cover in black
            break;

        case SIMON_DISPLAY_REGION_1: //yellow, shifted half over to right
            display_fillRect(SQUARE_X_OFFSET, 0, SQUARE_WIDTH, SQUARE_HEIGHT, DISPLAY_YELLOW);

            if(erase)
                display_fillRect(SQUARE_X_OFFSET, 0, SQUARE_WIDTH, SQUARE_HEIGHT, DISPLAY_BLACK);//erase is true, cover in black
            break;

        case SIMON_DISPLAY_REGION_2: //blue, shifted half down
            display_fillRect(0, SQUARE_Y_OFFSET, SQUARE_WIDTH, SQUARE_HEIGHT, DISPLAY_BLUE);

            if(erase)
                display_fillRect(0, SQUARE_Y_OFFSET, SQUARE_WIDTH, SQUARE_HEIGHT, DISPLAY_BLACK);//erase is true, cover in black
            break;

        case SIMON_DISPLAY_REGION_3: //green
            display_fillRect(SQUARE_X_OFFSET, SQUARE_Y_OFFSET, SQUARE_WIDTH, SQUARE_HEIGHT, DISPLAY_GREEN);

            if(erase)
                display_fillRect(SQUARE_X_OFFSET, SQUARE_Y_OFFSET, SQUARE_WIDTH, SQUARE_HEIGHT, DISPLAY_BLACK);//erase is true, cover in black
            break;

        default:
            printf(INVALID_REGION_ERROR); //invalid region. oops
            break;
        }
}


// I used a busy-wait delay (utils_msDelay) that uses a for-loop and just blocks until the time has passed.
// When you implement the game, you CANNOT use this function as we discussed in class. Implement the delay
// using the non-blocking state-machine approach discussed in class.
void simonDisplay_runTest(uint16_t touchCount) {
  display_init();  // Always initialize the display.
  char str[MAX_STR];   // Enough for some simple printing.
  uint8_t regionNumber;
  uint16_t touches = 0;
  // Write an informational message and wait for the user to touch the LCD.
  display_fillScreen(DISPLAY_BLACK);        // clear the screen.
  display_setCursor(0, display_height()/2); //
  display_setTextSize(TEXT_SIZE);
  display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);
  sprintf(str, "Touch and release to start the Simon demo.");
  display_println(str);
  display_println();
  sprintf(str, "Demo will terminate after %d touches.", touchCount);
  display_println(str);
  while (!display_isTouched());       // Wait here until the screen is touched.
  while (display_isTouched());        // Now wait until the touch is released.
  display_fillScreen(DISPLAY_BLACK);  // Clear the screen.
  simonDisplay_drawAllButtons();      // Draw all of the buttons.
  bool touched = false;           // Keep track of when the pad is touched.
  int16_t x, y;               // Use these to keep track of coordinates.
  uint8_t z;                  // This is the relative touch pressure.
  while (touches < touchCount) {  // Run the loop according to the number of touches passed in.
    if (!display_isTouched() && touched) {         // user has stopped touching the pad.
      simonDisplay_drawSquare(regionNumber, true); // Erase the square.
      simonDisplay_drawButton(regionNumber);       // DISPLAY_REDraw the button.
      touched = false;                                                          // Released the touch, set touched to false.
    } else if (display_isTouched() && !touched) {   // User started touching the pad.
      touched = true;                               // Just touched the pad, set touched = true.
      touches++;                                                                    // Keep track of the number of touches.
      display_clearOldTouchData();  // Get rid of data from previous touches.
      // Must wait this many milliseconds for the chip to do analog processing.
      utils_msDelay(TOUCH_PANEL_ANALOG_PROCESSING_DELAY_IN_MS);
      display_getTouchedPoint(&x, &y, &z);                  // After the wait, get the touched point.
      regionNumber = simonDisplay_computeRegionNumber(x, y);// Compute the region number.
      simonDisplay_drawSquare(regionNumber, false);     // Draw the square (erase = false).
    }
  }
  // Done with the demo, write an informational message to the user.
  display_fillScreen(DISPLAY_BLACK);        // clear the screen.
  display_setCursor(0, display_height()/2); // Place the cursor in the middle of the screen.
  display_setTextSize(2);                   // Make it readable.
  display_setTextColor(DISPLAY_RED, DISPLAY_BLACK);  // red is foreground color, black is background color.
  sprintf(str, "Simon demo terminated");    // Format a string using sprintf.
  display_println(str);                     // Print it to the LCD.
  sprintf(str, "after %d touches.", touchCount);  // Format the rest of the string.
  display_println(str);  // Print it to the LCD.

}
