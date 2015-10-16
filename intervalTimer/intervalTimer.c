/*
 * intervalTimer.c
 *
 *  Created on: Sep 16, 2015
 *      Author: skguth10, Seth Guthrie ECEn 330
 */

//goes to an adddres, and writes a value to it.

#include "intervalTimer.h"

uint32_t intervalTimer_whichTimer(uint32_t timerNumber)
{
    uint32_t baseADDR = 0;
        switch(timerNumber){
            case 0: //timer 0, addr 1
                baseADDR = XPAR_AXI_TIMER_0_BASEADDR;
                break;
            case 1: //timer 1, addr 1
                baseADDR = XPAR_AXI_TIMER_1_BASEADDR;
                break;
            case 2: //timer 2, addr 2
                baseADDR = XPAR_AXI_TIMER_2_BASEADDR;
                break;
            default: //baseADDR stays at 0, which is bad
                printf("invalid timer!");
                break;
        }
        return baseADDR;
}

uint32_t intervalTimer_init(uint32_t timerNumber)
{
    //looks at timerNumber and gives the write timer baseADDR
    uint32_t baseADDR = intervalTimer_whichTimer(timerNumber);


    //write a 0 to the TCSR0 register.
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR0_ADDR, INTERVAL_TIMER_CLEAR);
    //write a 0 to the TCSR1 register.
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR1_ADDR, INTERVAL_TIMER_CLEAR);

    //set the CASC bit and clear the UDT0 bit in the TCSR0 register (cascade mode and up counting).
    //The Casc bit is bit 11. See page 13 of Xilinx documenation. The Casc bit is apart of the TCSR0 register
    //clear the UDT0 bit in the TCSR0 register, makes the timer count up!
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR0_ADDR, INTERVAL_TIMER_CASC_UDT0_BIT);

    return INTERVAL_TIMER_PROCESS_OK;
}

uint32_t intervalTimer_reset(uint32_t timerNumber) //reset isn't working...if we run test twice, numbers way messed up
{
    //looks at timerNumber and gives the write timer baseADDR
    uint32_t baseADDR = intervalTimer_whichTimer(timerNumber);
    //remember each timer has 2 counters, so we have to set each to 0


    //store 0 into counter 0
    //write 0 into TLR0 register
    Xil_Out32(baseADDR + INTERVAL_TIMER_TLR0_ADDR, INTERVAL_TIMER_CLEAR);
    //write 1 into LOAD0 bit in the TCSR0
    //however, make sure you don't messed with the CASC and UDT0 bits which are 1 and 0 respectively
    // the load bit is bit 5. 1000-0001-0000  0x0810
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR0_ADDR, INTERVAL_TIMER_TCSR0_LOAD_CASC);


    //To store a 0 into counter 1, do the following:
    //write a 0 into the TLR1 register.
    Xil_Out32(baseADDR + INTERVAL_TIMER_TLR1_ADDR, INTERVAL_TIMER_CLEAR);
    //write a 1 into the LOAD1 bit of the TCSR1 register.
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR1_ADDR, INTERVAL_TIMER_TCSR0_LOAD_CASC);

    //we must also turn down the load bits after words, or it will keep loading zero
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR0_ADDR, INTERVAL_TIMER_CASC_UDT0_BIT);
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR1_ADDR, INTERVAL_TIMER_CASC_UDT0_BIT);

    return INTERVAL_TIMER_PROCESS_OK;
}

uint32_t intervalTimer_start(uint32_t timerNumber)
{
    //looks at timerNumber and gives the write timer baseADDR
    uint32_t baseADDR = intervalTimer_whichTimer(timerNumber);

   //To start the cascaded counter:
    //write a 1 to the ENT0 bit of the TCSR0 register. When you do this, you must not disturb the other bits in TCSR0.
    //the ENT0 bit  7
    Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR0_ADDR, INTERVAL_TIMER_TCSR0_CASC_ENT0); //will this set issues because the CASC bit is being 'refreshed'?

    return INTERVAL_TIMER_PROCESS_OK;

}
uint32_t intervalTimer_stop(uint32_t timerNumber)
{
    //looks at timerNumber and gives the write timer baseADDR
        uint32_t baseADDR = intervalTimer_whichTimer(timerNumber);

    //To stop the cascaded counter:
    //clear the ENT0 bit in the TCSR0 register. Make sure not to clear out the CASC bit when you do this
        Xil_Out32(baseADDR + INTERVAL_TIMER_TCSR0_ADDR, INTERVAL_TIMER_CASC_UDT0_BIT);
    //(or you will need to restore it)
    return INTERVAL_TIMER_PROCESS_OK;
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//MILESTONE 2

uint32_t intervalTimer_initAll()
{
    //calls each single function on each TIMER
    //this if fuction makes sure that each one returned 1, if it did, return 1
    //else, it failed, return 0

    if (intervalTimer_init(INTERVAL_TIMER_TIMER0) && intervalTimer_init(INTERVAL_TIMER_TIMER1) && intervalTimer_init(INTERVAL_TIMER_TIMER2))
        return INTERVAL_TIMER_PROCESS_OK;
    else
        return INTERVAL_TIMER_PROCESS_FAIL;
}

uint32_t intervalTimer_resetAll()
{
    //calls each single function on each TIMER
        //this if fuction makes sure that each one returned 1, if it did, return 1
        //else, it failed, return 0

        if (intervalTimer_reset(INTERVAL_TIMER_TIMER0) && intervalTimer_reset(INTERVAL_TIMER_TIMER1) && intervalTimer_reset(INTERVAL_TIMER_TIMER2))
            return INTERVAL_TIMER_PROCESS_OK;
        else
            return INTERVAL_TIMER_PROCESS_FAIL;
}


/*
 * runs test on single timer:
reset the counter, see if it reset by reading it
start the counter and read it a few times to make sure its actually changing the value
stop the counter and read it a few times to see that is not changing
 */
uint32_t intervalTimer_runTest(uint32_t timerNumber)
{
    //looks at timerNumber and gives the write timer baseADDR
        uint32_t baseADDR = intervalTimer_whichTimer(timerNumber);

        //where is the address for the actual counter? is it


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //TEST RESET


        // we reset the value, then we must wait and read, make sure its zero
        //reset
        intervalTimer_reset(timerNumber);
        //a bool that we'll falg as true if it actually reset, will be used in return function
        bool resetFlag = false;

        for (uint8_t i = 0; i < 10; i++) //what type should i be?
        {
            if ((Xil_In32(baseADDR + INTERVAL_TIMER_TCR0_ADDR) == INTERVAL_TIMER_CLEAR) && (Xil_In32(baseADDR + INTERVAL_TIMER_TCR1_ADDR) == INTERVAL_TIMER_CLEAR)) //both counters zero, we're good
                resetFlag = true;
            else
            {
                resetFlag = false; //they're not both zero, flag false and break
                break;
            }
        }


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //TEST START
        intervalTimer_start(timerNumber);
        //a bool we'll set as true if each number we check is greater than the previous
        bool startFlag = false;

        //check the counter values
        u32 oldValue0 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR0_ADDR); //
        u32 oldValue1 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR1_ADDR);

        for (uint8_t i = 0; i < 10; i++) //this loop will allow us to check the vlaues several times
        {
            u32 newValue0 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR0_ADDR);
            u32 newValue1 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR1_ADDR);

            //however, the new counter maynot always be bigger, so as long as it is the same or greather its ok
            if ((newValue0 > oldValue0) && (newValue1 >= oldValue1)) //if the new values are greater than the old values, we're good!
                startFlag = true;
            else
            {
                startFlag = false; //else they're not greather. set flag to false and break;
                break;
            }

            oldValue0 = newValue0;
            oldValue1 = newValue1;
        }

        //loop several times, making sure its greater each time

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //TEST STOP

        //stop the timer
        intervalTimer_stop(timerNumber);

        oldValue0 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR0_ADDR); //get new values for old, just after stopping
        oldValue1 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR1_ADDR);

        //a bool we'll set as true if the number is the same each time we check it
        bool stopFlag = false;

        for (uint8_t i = 0; i < 10; i++) //this loop will allow us to check the vlaues several times
        {
            u32 newValue0 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR0_ADDR); //get a new value that is later than old value
            u32 newValue1 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR1_ADDR);

            if ((newValue0 == oldValue0) && (newValue1 == newValue0)) //if the new values are the same than the old values, we're good!
                stopFlag = true;
            else
            {
                stopFlag = false; //else they're not the same. set flag to false and break;
                break;
            }
        }


    if (resetFlag && startFlag && stopFlag) //all three tests passed, it all passes
        return INTERVAL_TIMER_PROCESS_OK;
    else
        return INTERVAL_TIMER_PROCESS_FAIL;
}
uint32_t intervalTimer_testAll()
{
    //calls each single function on each TIMER
            //this if fuction makes sure that each one returned 1, if it did, return 1
            //else, it failed, return 0

            if (intervalTimer_runTest(INTERVAL_TIMER_TIMER0) && intervalTimer_runTest(INTERVAL_TIMER_TIMER1) && intervalTimer_runTest(INTERVAL_TIMER_TIMER2)) //all three tests for all three timers passed, so its good to go
                return INTERVAL_TIMER_PROCESS_OK;
            else
                return INTERVAL_TIMER_PROCESS_FAIL; //imma say this nice and slow, this things broke as !@#$
}

uint32_t intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber, double *seconds)
{
    //looks at timerNumber and gives the write timer baseADDR
    uint32_t baseADDR = intervalTimer_whichTimer(timerNumber);

    double processorSpeed = 0;
    //processor (or clk speed is important because we'll use it to take the counter value to convert to seconds
    switch(timerNumber) { //sees which timer we're looking at and look at its clock speed, and assigns it
    case 0:
        processorSpeed = XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
        break;
    case 1:
        processorSpeed = XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ;
        break;
    case 2:
        processorSpeed = XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ;
        break;
    }

    u32 counterValue0 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR0_ADDR); //look at value of counter0
    u32 counterValue1 = Xil_In32(baseADDR + INTERVAL_TIMER_TCR1_ADDR); //look at counter1 value

    //make 64 bit bit
    //shift value1 over 32 bits
    //combine them (mask?)
    //multiply by processorspeed!

    uint64_t counterValue0_64Bit = counterValue0; // LSBits

    uint64_t counterValue1_64Bit = counterValue1 << INTERVAL_TIMER_SHIFT_WIDTH; //most significant bits,2 must push over 32 bits(which is 20 in hex)

    uint64_t combinedCounterValue_64Bit = counterValue1_64Bit | counterValue0_64Bit; //the 2 64 bit numbers or'd together

    //dereference the seconds, cast as a double so that our seconds don't truncate.
    //it ticked for each clk cycle, so divide by that
    *seconds = ((double)(combinedCounterValue_64Bit) / processorSpeed);

    return INTERVAL_TIMER_PROCESS_OK;
}
