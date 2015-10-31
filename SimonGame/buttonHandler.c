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

uint8_t buttonHandler_getRegionNumber()
{

}

void buttonHandler_enable()
{

}

void buttonHandler_disable()
{

}

bool buttonHandler_releaseDetected()
{

}

void buttonHandler_tick()
{

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




