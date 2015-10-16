//Author: @Seth Guthrie, Created 9/16/15
#include "xparameters.h"
#include "xil_io.h"
#include <stdint.h>
#include <stdio.h>


//#define XPAR_AXI_TIMER_0_BASEADDR
//#define XPAR_AXI_TIMER_1_BASEADDR
//#define XPAR_AXI_TIMER_2_BASEADDR

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/* REGISTER INFO
 * TCSR0: this control/status register is used to control the cascaded 64-bit counter and to load values into the 32-bit counter.
TCSR1: you will use this control/status register only to load a 0 into counter 1 to reset it.
TLR0: you will need to set both 32-bit counters to zero in order to reset the timer. You do this for counter 0 by loading a 0 into this register and loading the contents of TLR0 into the counter.
TLR1: you load a 0 into this and load the contents of TLR1 into counter 1 to reset it and set it to 0 (just like counter 0).
TCR0: you read this register to find out the current value of counter 0.
TCR1: you read this register to find out the current value of counter 1.
 * */

//REGISTER ADDRESS:
#define INTERVAL_TIMER_TCSR0_ADDR 0x00 //control
#define INTERVAL_TIMER_TCSR1_ADDR 0x10 //control
#define INTERVAL_TIMER_TLR0_ADDR 0x04 //load
#define INTERVAL_TIMER_TLR1_ADDR 0x14 //load
#define INTERVAL_TIMER_TCR0_ADDR 0x08 //the register of the actual counter values. counter 0
#define INTERVAL_TIMER_TCR1_ADDR 0x14 //the register of the actual counter values. counter 1

//BITS TO SET
#define INTERVAL_TIMER_CLEAR 0x00
//the casc_bit must be turned on to allowed the 2 32-bit counters to cascade.
//the UDT0 is  bit 1 , so it must also be 0 to allow us to count up
//it is the 11 bit so... 1000-0000-0000
#define INTERVAL_TIMER_CASC_UDT0_BIT 0x0800
//when we change the load bit, we need casc to stay up. used in reset
#define INTERVAL_TIMER_TCSR0_LOAD_CASC 0x0820 //1000-0010-0000  0x0820
//we need to set the ENT0 bit, which starts the timer running, it is bit 7
#define INTERVAL_TIMER_TCSR0_CASC_ENT0 0x0880//1000-1000-0000

#define INTERVAL_TIMER_PROCESS_OK 1 //the return values for functions
#define INTERVAL_TIMER_PROCESS_FAIL 0

//this function will decide which base address to use for the other functions
//depending on which timer we're working with
uint32_t intervalTimer_whichTimer(uint32_t timerNumber);

//important to note that the init will not reset (set to 0) the counters, only set them up to cascade and count up
uint32_t intervalTimer_init(uint32_t timerNumber);
uint32_t intervalTimer_reset(uint32_t timerNumber);

uint32_t intervalTimer_start(uint32_t timerNumber);
uint32_t intervalTimer_stop(uint32_t timerNumber);

//since many of these functions are already written on a single timer basis, we only need call the single functions
//for each timer as such, the define belows used to itdenity radios
#define INTERVAL_TIMER_TIMER0 0
#define INTERVAL_TIMER_TIMER1 1
#define INTERVAL_TIMER_TIMER2 2
//Init all timers at the same time,
uint32_t intervalTimer_initAll();

//resets timers for all
uint32_t intervalTimer_resetAll();



//runs test on single timer:
//reset the counter, see if it reset by reading it
//start the counter and read it a few times to make sure its actually changing the value
//stop the counter and read it a few times to see that is not changing
uint32_t intervalTimer_runTest(uint32_t timerNumber);


// runs the runTest function on all radios, if all return true, we're good
uint32_t intervalTimer_testAll();

//takes in timer number and a pointer to a double
//sets seconds
uint32_t intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber, double *seconds);
//we need the frequency of processor which is xparameters as
/*
 *
 * #define XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ
 * #define XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ
 * */

#define INTERVAL_TIMER_SHIFT_WIDTH 0x20 //shift over 32 bits!. Used for printing out seconds in terms of the counters, we combine the 2 cascade counters
//by shifting one over 32 bits.





// Initializes the SWITCHES driver software and hardware. Returns one of the STATUS values defined above.
int switches_init();




