/*
 * clockMain2.c
 *
 *  Created on: Oct 6, 2015
 *      Author: skguth10
 */


#include "xparameters.h"
#include <stdio.h>
#include "supportFiles/leds.h"
#include "supportFiles/globalTimer.h"
#include "supportFiles/interrupts.h"
#include <stdbool.h>
#include <stdint.h>
#include "supportFiles/display.h"
#include "ticTacToeControl.h"

#define TOTAL_SECONDS 120
// The formula for computing the load value is based upon the formula from 4.1.1 (calculating timer intervals)
// in the Cortex-A9 MPCore Technical Reference Manual 4-2.
// Assuming that the prescaler = 0, the formula for computing the load value based upon the desired period is:
// load-value = (period * timer-clock) - 1
#define TIMER_PERIOD 150.0E-3
#define TIMER_CLOCK_FREQUENCY (XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 2)
#define TIMER_LOAD_VALUE ((TIMER_PERIOD * TIMER_CLOCK_FREQUENCY) - 1.0)

static uint32_t isr_functionCallCount = 0;

int main()
{

    // Init all interrupts (but does not enable the interrupts at the devices).
    // Prints an error message if an internal failure occurs because the argument = true.
    interrupts_initAll(true);
    interrupts_setPrivateTimerLoadValue(TIMER_LOAD_VALUE);
    printf("timer load value:%ld\n\r", (int32_t) TIMER_LOAD_VALUE);
    u32 privateTimerTicksPerSecond = interrupts_getPrivateTimerTicksPerSecond();
    printf("private timer ticks per second: %ld\n\r", privateTimerTicksPerSecond);
    interrupts_enableTimerGlobalInts();
    // Initialization of the clock display is not time-dependent, do it outside of the state machine.
    display_init();
    display_fillScreen(DISPLAY_BLACK);
    // Start the private ARM timer running.
    interrupts_startArmPrivateTimer();
    // Enable interrupts at the ARM.
    interrupts_enableArmInts();
    // The while-loop just waits until the total number of timer ticks have occurred before proceeding.
    while (interrupts_isrInvocationCount() < (TOTAL_SECONDS * privateTimerTicksPerSecond));
    // All done, now disable interrupts and print out the interrupt counts.
    interrupts_disableArmInts();
    printf("isr invocation count: %ld\n\r", interrupts_isrInvocationCount());
    printf("internal interrupt count: %ld\n\r", isr_functionCallCount);
    return 0;
}

// The clockControl_tick() function is now called directly by the timer interrupt service routine.
void isr_function() {
    // Add the necessary code here.
    ticTacToeControl_tick();
    isr_functionCallCount++;
}

//int main()
//{
//    minimax_board_t board1;  // Board 1 is the main example in the web-tutorial that I use on the web-site.
//      board1.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//      board1.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board1.squares[0][2] = MINIMAX_PLAYER_SQUARE;
//      board1.squares[1][0] = MINIMAX_PLAYER_SQUARE;
//      board1.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board1.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board1.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//      board1.squares[2][1] = MINIMAX_OPPONENT_SQUARE;
//      board1.squares[2][2] = MINIMAX_OPPONENT_SQUARE;
//
//      minimax_board_t board2;
//      board2.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//      board2.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board2.squares[0][2] = MINIMAX_PLAYER_SQUARE;
//      board2.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board2.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board2.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board2.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//      board2.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board2.squares[2][2] = MINIMAX_OPPONENT_SQUARE;
//
//      minimax_board_t board3;
//      board3.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//      board3.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board3.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board3.squares[1][0] = MINIMAX_OPPONENT_SQUARE;
//      board3.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board3.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board3.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//      board3.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board3.squares[2][2] = MINIMAX_PLAYER_SQUARE;
//
//      minimax_board_t board4;
//      board4.squares[0][0] = MINIMAX_OPPONENT_SQUARE;
//      board4.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board4.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board4.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board4.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board4.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board4.squares[2][0] = MINIMAX_PLAYER_SQUARE;
//      board4.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board4.squares[2][2] = MINIMAX_PLAYER_SQUARE;
//
//      minimax_board_t board5;
//      board5.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//      board5.squares[0][1] = MINIMAX_PLAYER_SQUARE;
//      board5.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board5.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board5.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
//      board5.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board5.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//      board5.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board5.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
//     uint8_t row, column;
//
//
////     minimax_computeNextMove(&board2, true, &row, &column);
////     printf("next move for board2: (%d, %d)\n\r", row, column);
//     minimax_computeNextMove(&board3, true, &row, &column);
//     printf("next move for board3: (%d, %d)\n\r", row, column);
//     minimax_computeNextMove(&board4, false, &row, &column);
//     printf("next move for board4: (%d, %d)\n\r", row, column);
//     minimax_computeNextMove(&board5, false, &row, &column);
//     printf("next move for board5: (%d, %d)\n\r", row, column);
//
//     minimax_computeNextMove(&board1, true, &row, &column);
//     printf("next move for board1: (%d, %d)\n\r", row, column);


//    minimax_board_t board6;  //board6 should be the same as board7
//      board6.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//      board6.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board6.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
//      minimax_board_t board7; //we get the correct response here
//      board7.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//      board7.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board7.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
//      minimax_board_t board8;
//      board8.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//      board8.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[1][1] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board8.squares[2][2] = MINIMAX_EMPTY_SQUARE;
//
//      minimax_board_t board9;
//      board9.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//      board9.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board9.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board9.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board9.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
//      board9.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board9.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//      board9.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board9.squares[2][2] = MINIMAX_PLAYER_SQUARE;
//
//      minimax_board_t board10;
//      board10.squares[0][0] = MINIMAX_PLAYER_SQUARE;
//      board10.squares[0][1] = MINIMAX_EMPTY_SQUARE;
//      board10.squares[0][2] = MINIMAX_EMPTY_SQUARE;
//      board10.squares[1][0] = MINIMAX_EMPTY_SQUARE;
//      board10.squares[1][1] = MINIMAX_OPPONENT_SQUARE;
//      board10.squares[1][2] = MINIMAX_EMPTY_SQUARE;
//      board10.squares[2][0] = MINIMAX_EMPTY_SQUARE;
//      board10.squares[2][1] = MINIMAX_EMPTY_SQUARE;
//      board10.squares[2][2] = MINIMAX_PLAYER_SQUARE;
//
//
//
//
//     minimax_computeNextMove(&board6, false, &row, &column);
//     printf("next move for board6: (%d, %d)\n\r", row, column);
//
//     minimax_computeNextMove(&board7, false, &row, &column);
//     printf("next move for board7: (%d, %d)\n\r", row, column);
//
//
//     //minimax_computeNextMove(&board8, false, &row, &column);
//     //printf("next move for board8: (%d, %d)\n\r", row, column);
//
//     //minimax_computeNextMove(&board9, false, &row, &column);
//     //printf("next move for board9: (%d, %d)\n\r", row, column);
//    // minimax_computeNextMove(&board10, false, &row, &column);
//     //printf("next move for board10: (%d, %d)\n\r", row, column);
//
//}
//
//void isr_function() {}
