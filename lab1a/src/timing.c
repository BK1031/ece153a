/*
 *timing.c: simple starter application for lab 1A and 1B
 *
 */

#include <stdio.h>		// Used for printf()
#include <stdlib.h>		// Used for rand()
#include "xparameters.h"	// Contains hardware addresses and bit masks
#include "xil_cache.h"		// Cache Drivers
#include "xintc.h"		// Interrupt Drivers
#include "xtmrctr.h"		// Timer Drivers
#include "xtmrctr_l.h" 		// Low-level timer drivers
#include "xil_printf.h" 	// Used for xil_printf()
#include "extra.h" 		// Provides a source of bus contention
#include "xgpio.h" 		// LED driver, used for General purpose I/i

#define NUMBER_OF_TRIALS 150
#define NUMBER_OF_BINS 15
#define BUFFER_SIZE (1024*1024)
unsigned int buffer[BUFFER_SIZE]; //buffer for read/write operations to the DDR memory

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL 1

void histogram(void); // This function creates a histogram for the measured data

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
XGpio Gpio; /* The Instance of the GPIO Driver used for LED 0 */

/*
 * This globally declared array stores the
 * number of clock cycles for all the trials.
 * With global declaration, it is stored in the data segment of the
 * memory. Declaring this large array locally may cause a stack overflow.
 */
int numClockCycles[NUMBER_OF_TRIALS];

//Stores the number of samples in each bin
int histData[NUMBER_OF_BINS];

//BaseAddr points to the base (byte) address of the DDR2 Memory
u8 * BaseAddr = (u8 *) XPAR_MIG7SERIES_0_BASEADDR;

/*
 * REPEAT is a somewhat dangerous macro that makes multiple statements from an
 * input string. The danger is statements like if() without {}... Here this is
 * no problem, but beware if you use it elsewhere -- macro bugs are often very
 * hard to debug.
 */
#define REPEAT_5(X) X;X;X;X;X
#define REPEAT_10(X) REPEAT_5(X);REPEAT_5(X)
#define REPEAT_15(X) REPEAT_10(X);REPEAT_5(X)
#define REPEAT_20(X) REPEAT_15(X);REPEAT_5(X)
#define REPEAT_25(X) REPEAT_20(X);REPEAT_5(X)
#define REPEAT_40(X) REPEAT_15(X);REPEAT_25(X)

int a = 3;
int b = 4;
int c;

float d = 3.14;
float e = 2.71;
float f;

u32 Addr;
volatile unsigned int Data;

int main() {
	Xil_ICacheInvalidate();
	Xil_ICacheEnable();
	Xil_DCacheInvalidate();
	Xil_DCacheEnable();
	print("---Entering main---\n\r");
	int i = 0;
	int timer_val_before; // Used to store the timer value before executing the operation being timed

	// Extra Method contains an interrupt routine which is set to go off at timed intervals
	extra_method();

	// TIMER RESET CODE
	// Turn off the timer
	XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, 0);
	// Put a zero in the load register
	XTmrCtr_SetLoadReg(XPAR_TMRCTR_0_BASEADDR, 1, 0);
	// Copy the load register into the counter register
	XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, XTC_CSR_LOAD_MASK);
	// Enable (start) the timer
	XTmrCtr_SetControlStatusReg(XPAR_TMRCTR_0_BASEADDR, 1, XTC_CSR_ENABLE_TMR_MASK);
	// END TIMER RESET CODE

	// INITIALIZATION FOR AXI GPIO LED PORT
	XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_LED_DEVICE_ID);

	for (i = 0; i < NUMBER_OF_TRIALS; i++) {

		// Store the timer value before executing the operation being timed
		timer_val_before = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, 1);

		// Enter the line of Code to time.
		integerAddition();

		numClockCycles[i] = XTmrCtr_GetTimerCounterReg(XPAR_TMRCTR_0_BASEADDR, 1) - timer_val_before; // Stores the time to execute the operation

	}

	// Prints the collected data
	for (i = 0; i < NUMBER_OF_TRIALS; i++) {
		xil_printf("%d,%d\n\r", i, numClockCycles[i]);
	}

	histogram(); // Creates a histogram for the measured data

}

// 5.1 - Timing of addition using integers
void integerAddition() {
	REPEAT_5(c = a + b);
	// REPEAT_15(c = a + b);
	// REPEAT_25(c = a + b);
	// REPEAT_40(c = a + b);
}

// 5.2 - Timing of addition using floating point
void floatAddition() {
	REPEAT_5(f = d + e);
	// REPEAT_15(f = d + e);
	// REPEAT_25(f = d + e);
	// REPEAT_40(f = d + e);
}

// 5.3 - Timing of writing the LEDs to turn on or off
void writeLED() {
	REPEAT_5(XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x1));
	// REPEAT_15(XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x1));
	// REPEAT_25(XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x1));
	// REPEAT_40(XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x1));
}

// 5.4 - Timing of reading a word from the DDR2 memory at a random location
void readDDR() {
	Addr = rand() % BUFFER_SIZE; // Will be used to access a random buffer index
	REPEAT_5(Data = buffer[Addr]);
	// REPEAT_15(Data = buffer[Addr]);
	// REPEAT_25(Data = buffer[Addr]);
	// REPEAT_40(Data = buffer[Addr]);
}

// 5.5 - Timing of writing to the USB Port (ONLY RUN MAIN LOOP 1000 TIMES)
// (a) Write a floating point number using printf()
void writeFloatUSB() {
	REPEAT_5(printf("%f\n\r", 0.1));
	// REPEAT_15(printf("%f\n\r", 0.1));
	// REPEAT_25(printf("%f\n\r", 0.1));
	// REPEAT_40(printf("%f\n\r", 0.1));
}
// (b) Write a string of 10 characters using xil printf()
void writeStringUSB() {
	REPEAT_5(xil_printf("helloworld\n\r"));
	// REPEAT_15(xil_printf("helloworld\n\r"));
	// REPEAT_25(xil_printf("helloworld\n\r"));
	// REPEAT_40(xil_printf("helloworld\n\r"));
}

/*
 *
 * histogram is a function that runs locally to reduce the data for longer tests
 * i.e. you don't want to copy 10,000 values
 * you need to modify it for your own use to make the output statistically valuable
 */
void histogram(void) {

	int min, max, binSize, binIndex;

	int i;

	// min and max initialized
	min = numClockCycles[0];
	max = numClockCycles[0];

	// find the min and max values
	for (i = 0; i < NUMBER_OF_TRIALS; i++) {
		if (numClockCycles[i] < min)
			min = numClockCycles[i];
		if (numClockCycles[i] > max)
			max = numClockCycles[i];
	}

	binSize = (max - min) / NUMBER_OF_BINS;

	// Bin number for each data element is found here
	for (i = 0; i < NUMBER_OF_TRIALS; i++) {
		binIndex = 0;
		if (binSize > 0) {
			binIndex = (numClockCycles[i] - min) / binSize;
			if (binIndex >= NUMBER_OF_BINS)
				binIndex = NUMBER_OF_BINS - 1;
			histData[binIndex]++;
		} else {
			// if there is no variance in the data all values are assigned to bin 0
			histData[0] = NUMBER_OF_TRIALS;
			break;
		}
	}
	// Prints the number of elements in each bin
	for (i = 0; i < NUMBER_OF_BINS; i++) {
		xil_printf("Bin %d: %d\n\r", i, histData[i]);
	}

	xil_printf("Done!\n\r");

}

