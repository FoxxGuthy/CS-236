//Seth Guthrie
#include <stdint.h> // for our custom variables
#include "xparameters.h" // to access predefined base ADDR
#include "xil_io.h" // for the functions used in writing tristate andreading swtiches
#include "supportFiles/leds.h" // to access the LEDS
#include "buttons.h" // to access the tristate offset, and

//for use in init func, will return if init passes or failed
#define SWITCHES_INIT_STATUS_OK 1
#define SWITCHES_INIT_STATUS_FAIL 0

//the memeory address of the sweithc
#define SWITCHES_GPIO_BASE_ADDR XPAR_SLIDE_SWITCHES_BASEADDR
//Seth Guthrie
//these are the bits where individual swithces are stored
#define SWITCHES_ALL_SWITCHES_ON 0x0000000F
#define SWITCHES_SW0 0x00000001 //...0001
#define SWITCHES_SW1 0x00000002 //...0010
#define SWITCHES_SW2 0x00000004 //...0100
#define SWITCHES_SW3 0x00000008 //...1000

#define SWITCHES_OFFSET 0x04
#define SWITCHES_TRISTATE_OFF 0x01



// Initializes the SWITCHES driver software and hardware. Returns one of the STATUS values defined above.
int switches_init();



// Returns the current value of all 4 SWITCHESs as the lower 4 bits of the returned value.
// bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
int32_t switches_read();

// Runs a test of the switches. As you slide the switches, LEDs directly above the switches will illuminate.
// The test will run until all switches are slid upwards. When all 4 slide switches are slid upward,
// this function will return.
void switches_runTest();
