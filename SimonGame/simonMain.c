/*
 * simonMain.c
 *
 *  Created on: Oct 31, 2015
 *      Author: skguth10
 */

#include "SimonDisplay.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h" // for testing purposes

int main()
{
    simonDisplay_runTest(5);
}

void isr_function()
{
    //empty for now
}

