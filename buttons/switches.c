//Seth Guthrie
#include "switches.h"


int switches_init()
{
    //turn off tri?
    //GPIO_TRI register
    // tri must be off when functioning as input
    // tri must be on when functioning as output

    //the init will never fail
    //goes to the base address, plus 4, which is where the tristate is, then set it to value 1, which turns it off
    Xil_Out32(SWITCHES_GPIO_BASE_ADDR + SWITCHES_OFFSET, SWITCHES_TRISTATE_OFF);

    return SWITCHES_INIT_STATUS_OK;
}

//returns the value of the switches, goes to the swithces base addrs and returns its value
int32_t switches_read()
{
    return Xil_In32(SWITCHES_GPIO_BASE_ADDR);
}

void switches_runTest()
{
    //initialize the LEDS
    leds_init(0);

    //initalize the switches, aka turn off tristate
    switches_init();


    int32_t switchValue = 0;

    //while not all the swtiches are up, continue to test
    //we mask the switchValue with all switches so we just see those bits
    while ((switchValue & SWITCHES_ALL_SWITCHES_ON) != SWITCHES_ALL_SWITCHES_ON)
    {
        switchValue = switches_read();
        //if switch0, led 0 on
        if ((switchValue & SWITCHES_SW0) == SWITCHES_SW0)
        {
            leds_write(SWITCHES_SW0); // this might cause problem, because it specifically turns off the other LEDS
        }
        //if switch1, led 1 on
        if ((switchValue & SWITCHES_SW1) == SWITCHES_SW1)
        {
            leds_write(SWITCHES_SW1); // this might cause problem, because it specifically turns off the other LEDS
        }
            //if switch2, led 2 on
        if ((switchValue & SWITCHES_SW2) == SWITCHES_SW2)
        {
            leds_write(SWITCHES_SW2); // this might cause problem, because it specifically turns off the other LEDS
        }
            //if switch3, led 3 on
        if ((switchValue & SWITCHES_SW3) == SWITCHES_SW3)
        {
            leds_write(SWITCHES_SW3); // this might cause problem, because it specifically turns off the other LEDS
        }
        else //no LEDS on
        {
            //when we put a switch down, turn off that LED
            leds_write(0);
        }
    }
    //turn off all LEDS, test is over
    leds_write(0);
}

