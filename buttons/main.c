//Seth Guthrie
#include "buttons.h"
#include "switches.h"



int main() {

    // runs button test, ending when all 4 are pressed
  buttons_runTest();

  // runs switch test, ending when all 4 are switched up
  switches_runTest();

  return 0;
}


// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function() {
    // Empty for now.
}
