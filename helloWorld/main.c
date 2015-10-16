
// Print out "hello world" on both the console and the LCD screen.

#include <stdio.h>
#include "supportFiles/display.h"
int main() {
	display_init();  // Must init all of the software and underlying hardware for LCD.
	display_fillScreen(DISPLAY_BLACK);  // Blank the screen.
	display_setCursor(0, 0);            // The upper left of the LCD screen.
	display_setTextColor(DISPLAY_RED);  // Make the text red.
	display_setTextSize(2);             // Make the text a little larger.
	//display_println("hello world (on the LCD)!");    // This actually prints the string to the LCD.
	printf("hello world!\n\r");  // This prints on the console.

	display_drawLine(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0X07E0);
	display_drawLine(0, DISPLAY_HEIGHT, DISPLAY_WIDTH, 0, 0X07E0);

	display_drawCircle(DISPLAY_WIDTH/3, DISPLAY_HEIGHT/2, 15, 0XF800);
	display_fillCircle((2*DISPLAY_WIDTH/3), DISPLAY_HEIGHT/2, 15, 0XF800);

	display_fillTriangle(160, 100, 140, 60, 180, 60, 0xFFE0);
	display_drawTriangle(160, 140, 140, 200, 180, 200, 0xFFE0);
}

// Yellow 0xFFE0
// Green 0X07E0
// Red 0XF800



// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function() {
    // Empty for now.
}

