//Seth Guthrie
#include "buttons.h"

#include "stdio.h"

int buttons_init()
{


    //creat pointer that points to Tristate memory
    uint32_t *tristateMem = (uint32_t *) BUTTONS_BASEADDR + BUTTONS_OFFSET;

    //turn off the tristate by assigning its value to 1
    *tristateMem = BUTTONS_TRISTATE_OFF;

    //Check that the tristate value was acutally set to 1
    //if it was, return status ok
    //else, we didn't initialize correctly. FAIL
    if (*tristateMem == BUTTONS_TRISTATE_OFF)
        return BUTTONS_INIT_STATUS_OK;
    else
        return BUTTONS_INIT_STATUS_FAIL;
}

int32_t buttons_read()
{
    //make memory address a ptr
    uint32_t *addrPtr = (uint32_t *) BUTTONS_BASEADDR;

    //this is a 32 bit value, the button values are the lower 4 bits [0-3]
    return *addrPtr;
}

void buttons_runTest()
{
    //calls the init function so we can turn off the tristate and read from the buttons
    buttons_init();

    // Must init all of the software and underlying hardware for LCD.
    display_init();

    //if we change the screen everytime, we can see it refresh, instead of holding steady
    //these values will compare the old and new button readings, so we can see if we have to change
    //only clear and draw buttons when we really need to.
    uint32_t newBTNValue = 0;
    uint32_t oldBTNValue = 0;


    do {
        //set old to new, and give newBTNValue the new read
        oldBTNValue = newBTNValue;
        newBTNValue = buttons_read();

        if (oldBTNValue == newBTNValue)
        {
            //its all the same! do nothing.
            //if we constantly recheck the screen and redraw, we see it flashing
            //to avoid this we only change the screen if the old and new values have changed
        }

        else
        {
            // if button0 pressed, mask it with 32 bit 1 so we can just look at that bit
            if ((newBTNValue & BUTTONS_BTN0) == BUTTONS_BTN0)
            {

                //draw a yellow rectangle, starting at 3/4 over, 1/4 width wide, and 1/2 height tall
                display_fillRect(BUTTONS_BTN0_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_YELLOW);


                //sets where we want the text to display, which is the middle of each rectangle
                display_setCursor(BUTTONS_CURSOR_BTN0, BUTTONS_CURSOR_HEIGHT);
                //we also want to increase the text size
                display_setTextSize(BUTTONS_TEXT_SIZE);
                //set the text color:
                display_setTextColor(DISPLAY_BLACK);

                //print the button name!
                display_println(BUTTONS_LABEL_BTN0);
            }
            else
            {
                //the button isn't pressed anymore, clear that part of the screen
                display_fillRect(BUTTONS_BTN0_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK);
            }

            // if button1 pressed, mask it so only bit1 is what we look at. 0010 == 2
            if ((newBTNValue & BUTTONS_BTN1) == BUTTONS_BTN1)
            {
                //draw a green rectangle, starting at 1/2 over, 1/4 width wide, and 1/2 height tall
                display_fillRect(BUTTONS_BTN1_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_GREEN);

                //set the text color:
                display_setTextColor(DISPLAY_BLACK);
                //sets where we want the text to display, which is the middle of each rectangle
                display_setCursor(BUTTONS_CURSOR_BTN1, BUTTONS_CURSOR_HEIGHT);
                //we also want to increase the text size
                display_setTextSize(BUTTONS_TEXT_SIZE);

                //print the button name!
                display_println(BUTTONS_LABEL_BTN1);
            }
            else
            {
                //the button isn't pressed anymore, clear that part of the screen
                display_fillRect(BUTTONS_BTN1_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK);
            }

            //LSB after mask should be 0100 which is 4
            if ((newBTNValue & BUTTONS_BTN2) == BUTTONS_BTN2)
            {
                //draw a RED rectangle, starting at 1/4 over, 1/4 width wide, and 1/2 height tall
                display_fillRect(BUTTONS_BTN2_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_RED);

                //set the text color:
                display_setTextColor(DISPLAY_WHITE);
                //sets where we want the text to display, which is the middle of each rectangle
                display_setCursor(BUTTONS_CURSOR_BTN2, BUTTONS_CURSOR_HEIGHT);
                //we also want to increase the text size
                display_setTextSize(BUTTONS_TEXT_SIZE);

                //print the button name!
                display_println(BUTTONS_LABEL_BTN2);
            }
            else
            {
                //the button isn't pressed anymore, clear that part of the screen
                display_fillRect(BUTTONS_BTN2_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK);
            }

            // LSB should be 1000 which is 8!
            if ((newBTNValue & BUTTONS_BTN3) == BUTTONS_BTN3)
            {
                //draw a cyan rectangle, starting at top left corner, goes 1/4 of screen wide, and 1/2 height tall
                display_fillRect(BUTTONS_BTN3_REC_START, BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_CYAN);

                //set the text color
                display_setTextColor(DISPLAY_WHITE);
                //sets where we want the text to display, which is the middle of each rectangle
                display_setCursor(BUTTONS_CURSOR_BTN3, BUTTONS_CURSOR_HEIGHT);
               //we also want to increase the text size
               display_setTextSize(BUTTONS_TEXT_SIZE);

               //print the button name!
               display_println(BUTTONS_LABEL_BTN3);
            }
            else
            {
                //the button isn't pressed anymore, clear that part of the screen
                display_fillRect(BUTTONS_BTN3_REC_START,BUTTONS_REC_START_HEIGHT, BUTTONS_RECTANGLE_WIDTH, BUTTONS_RECTANGLE_HEIGHT, DISPLAY_BLACK);
            }

        }

    } while ((newBTNValue & BUTTONS_ALLBTN) != BUTTONS_ALLBTN); //while the 4 lower bits != 1111, 0x000F

    //clear the screen after the test has completed
    display_fillScreen(DISPLAY_BLACK);
}
