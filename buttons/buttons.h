//Seth Guthrie
#include <stdint.h>
#include "supportFiles/display.h"
#include "xparameters.h"

//These are for use when looking at the value stored at the Base Addr. When the value is masked (&) with
//these we'll see if a specific button is pressed, the values are the last 4 bits of 32 bit number
//
#define BUTTONS_BTN0 0x1 //...0001

#define BUTTONS_BTN1 0x2//...0010

#define BUTTONS_BTN2 0x04 //...0100

#define BUTTONS_BTN3 0x08 //...1000

//heaven forbid you should ever want to change the actual text, but if so you can now do it in one fell swoop
//with these nifty #defines
#define BUTTONS_LABEL_BTN0 "BTN0"
#define BUTTONS_LABEL_BTN1 "BTN1"
#define BUTTONS_LABEL_BTN2 "BTN2"
#define BUTTONS_LABEL_BTN3 "BTN3"

//used when we check if all 4 buttons are pressed, if we mask this with the value, stop looping
#define BUTTONS_ALLBTN 0x0F

// these define the width and height of the rectangles displayed when buttons are pressed
#define BUTTONS_RECTANGLE_WIDTH (DISPLAY_WIDTH/4)
#define BUTTONS_RECTANGLE_HEIGHT (DISPLAY_HEIGHT/2)

// These numbers are for use in the buttons_init() function. will tell us if we initialize correctly
#define BUTTONS_INIT_STATUS_OK 1
#define BUTTONS_INIT_STATUS_FAIL 0

// these are the values for where the text will be displayed for each button.
#define BUTTONS_CURSOR_BTN0 (DISPLAY_WIDTH*3/4 + 20)
#define BUTTONS_CURSOR_BTN1 ((DISPLAY_WIDTH)/2 + 20)
#define BUTTONS_CURSOR_BTN2 (DISPLAY_WIDTH/4 + 20)
#define BUTTONS_CURSOR_BTN3 (20) //just a little buffer to push it off the edge
#define BUTTONS_CURSOR_HEIGHT (DISPLAY_HEIGHT/4)
//also associated with text
#define BUTTONS_TEXT_SIZE 2 //up 2 from default


//these define where the rectangles will begin to be drawn
#define BUTTONS_BTN0_REC_START (DISPLAY_WIDTH*3/4)
#define BUTTONS_BTN1_REC_START ((DISPLAY_WIDTH)/2)
#define BUTTONS_BTN2_REC_START (DISPLAY_WIDTH/4)
#define BUTTONS_BTN3_REC_START 0
#define BUTTONS_REC_START_HEIGHT 0



#define BUTTONS_TRISTATE_OFF 0x01

//the tristate is offset by 4
#define BUTTONS_OFFSET 0x04

 //sets BUTTONS_BASE ADDR to 0x41240000
#define BUTTONS_BASEADDR XPAR_PUSH_BUTTONS_BASEADDR

// Initializes the button driver software and hardware. Returns one of the defined status values (above).
// to be able to read from the buttons, we must turn off the tristate, that's all this function will do.
int buttons_init();

// Returns the current value of all 4 buttons as the lower 4 bits of the returned value.
// bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
int32_t buttons_read();
    // read buttons, returns number at base address

// Runs a test of the buttons. As you push the buttons, graphics and messages will be written to the LCD
// panel. The test will until all 4 pushbuttons are simultaneously pressed.
void buttons_runTest();

// This function is to draw the rectangles for each button press.
void buttons_drawBTN(int32_t buttonsValue, char clear);
