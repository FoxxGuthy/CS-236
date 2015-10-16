/*
 * test.c
 *
 *  Created on: Oct 5, 2015
 *      Author: skguth10
 */


#include "clockControl.h"

#define CLOCK_CONTROL_TIMER_PERIOD 50.0E-3

//if we use a different period besides 50e-3 these numbers don't evail to even ints, and so float division screw it all up
#define ADTIMER_EXPIRE ((1.0/CLOCK_CONTROL_TIMER_PERIOD)/20.0)//40-50m/s this so the touch screen can settle
#define AUTOTIMER_EXPIRE  ((1.0/CLOCK_CONTROL_TIMER_PERIOD)/2.0)//500m/s
#define RATETIMER_EXPIRE ((1.0/CLOCK_CONTROL_TIMER_PERIOD)/10.0) //100m/s

static uint8_t secondsTimer = 0; //counts ticks for when to inc the seconds
#define SECONDSTIMER_MAX (1/CLOCK_CONTROL_TIMER_PERIOD) //how many ticks per second

static uint8_t adTimer = 0; //counts up to 50m/s. it allow the touch to settle
static uint8_t rateTimer = 0; //counts up to 10m/s. every 10m/s it incs
static uint8_t autoTimer = 0; //counts up to 500m/s. how long touch is held until it auto incs

enum CLK_States { CLK_init_st, CLK_wait_st, CLK_waiting_for_touch_st, CLK_ad_timer_running_st, CLK_auto_timer_running_st,
    CLK_rate_timer_running_st, CLK_rate_timer_expired_st
} CLK_State;

void clockControl_tick() //the state machine
{

    switch (CLK_State) //transitions
    {
    case CLK_init_st: //initial state
        //printf("init state, going to CLK_wait_st\n\r");
        CLK_State = CLK_wait_st; //leave init state
        break;
    case CLK_wait_st:
        if (display_isTouched())
        {
            //printf("CLK_waiting_for_touch_st\n\r");
            CLK_State = CLK_waiting_for_touch_st;
        }

        break;
    case CLK_waiting_for_touch_st:

        secondsTimer++; //keep track of time
        if (secondsTimer == SECONDSTIMER_MAX) // if we have as many ticks as are in a second, we're good, advance!
        {
            clockDisplay_advanceTimeOneSecond(); //advance time
            secondsTimer = 0; //reset
        }
        if (display_isTouched())
        {
            //printf("CLK_ad_timer_running_st\n\r");
            CLK_State = CLK_ad_timer_running_st; //change the state

            display_clearOldTouchData(); //perform mealy action
        }

        break;
    case CLK_ad_timer_running_st:
        //printf("c\n\r");
        if (!display_isTouched() && adTimer == ADTIMER_EXPIRE) //path conditions 1
        {
            //printf("CLK_waiting_for_touch_st\n\r");
            CLK_State = CLK_waiting_for_touch_st; //move back to state 1
            clockDisplay_performIncDec(); //change the clock
        }
        if (display_isTouched() && adTimer == ADTIMER_EXPIRE) //path conditions 2
        {
            //printf("CLK_auto_timer_running_st\n\r");
            CLK_State = CLK_auto_timer_running_st; //advance to next state
        }
        break;
    case CLK_auto_timer_running_st:
        //printf("d\n\r");
        if (!display_isTouched()) //transtion 1
        {
            //printf("CLK_waiting_for_touch_st\n\r");
            CLK_State = CLK_waiting_for_touch_st; //change state
            clockDisplay_performIncDec(); //perform mealy action
        }
        if (display_isTouched() && autoTimer == AUTOTIMER_EXPIRE) //transition 2
        {
            //printf("CLK_rate_timer_running_st\n\r");
            CLK_State = CLK_rate_timer_running_st; //change state
            clockDisplay_performIncDec(); //perform mealy action
        }
        break;
    case CLK_rate_timer_running_st:
        //printf("e\n\r");
        if (!display_isTouched()) //transtion condition 1
        {
            //printf("CLK_waiting_for_touch_st\n\r");
            CLK_State = CLK_waiting_for_touch_st; //change state
        }
        if (display_isTouched() && rateTimer == RATETIMER_EXPIRE) //transition condition 2
        {
            //printf("CLK_rate_timer_expired_st\n\r");
            CLK_State = CLK_rate_timer_expired_st; //change state
        }
        break;
    case CLK_rate_timer_expired_st:

        if (display_isTouched()) //transition condition 1
        {
            //printf("CLK_rate_timer_running_st\n\r");
            CLK_State = CLK_rate_timer_running_st; //change state
            clockDisplay_performIncDec(); //mealy action
        }
        if (!display_isTouched()) //condition 2
        {
            //printf("CLK_waiting_for_touch_st\n\r");
            CLK_State = CLK_waiting_for_touch_st; //change the state
        }
        break;
    default:
        CLK_State = CLK_init_st; //something broke return to init!
        break;
    }

    switch (CLK_State) //state actions
    {
    case CLK_waiting_for_touch_st:
        adTimer = 0; //just waiting, these timers are about touch, so reset
        autoTimer = 0;
        rateTimer = 0;
        break;
    case CLK_ad_timer_running_st:
        adTimer++; //
        break;
    case CLK_auto_timer_running_st:
        autoTimer++;
        break;
    case CLK_rate_timer_running_st:
        rateTimer++;
        break;
    case CLK_rate_timer_expired_st:
        rateTimer = 0; //this counts how many ticks are in 10ms which is how often we inc/dec when holding
        break;
    default:
        break;
    }
}

