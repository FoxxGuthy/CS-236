/*
 * globals.c
 *
 *  Created on: Nov 4, 2015
 *      Author: skguth10
 */

#include "globals.h"

static uint8_t globalSequence[GLOBALS_MAX_FLASH_SEQUENCE];
static uint16_t currentSequenceLegnth = 0;


void globals_setSequence(const uint8_t sequence[], uint16_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        globalSequence[i] = sequence[i]; //copy sequence into global
    }
}

uint8_t globals_getSequenceValue(uint16_t index)
{
    return globalSequence[index]; // This returns the value of the sequence at the index.
}

void globals_setSequenceIterationLength(uint16_t length)
{
    currentSequenceLegnth = length; //set the sequence length
}

uint16_t globals_getSequenceIterationLength()
{
    return currentSequenceLegnth; //return the length
}
