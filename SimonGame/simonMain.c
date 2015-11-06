/*
 * simonMain.c
 *
 *  Created on: Oct 31, 2015
 *      Author: skguth10
 */

#include "simonDisplay.h"
#include "buttonHandler.h"
#include "flashSequence.h"
#include "verifySequence.h"
#include "supportFiles/display.h"
#include "supportFiles/utils.h" // for testing purposes

int main()
{
    verifySequence_runTest();
}

void isr_function()
{
    //empty for now
}

