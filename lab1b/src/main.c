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
#include "sevenSeg.h"
#include <stdbool.h>

XIntc sys_intc;
XTmrCtr sys_tmrctr;
XGpio sys_gpio;

static u16 global_intr_mask;

#define BUTTON_CHANNEL 1

unsigned int tick = 0;
unsigned int count = 0;

bool paused = false;
// 0 - count down, 1 - count up
int direction = 1;


void timer_handler() {
	// This is the interrupt handler function
	// Do not print inside of this function.
	Xuint32 ControlStatusReg;
	/*
	 * Read the new Control/Status Register content.
	 */
	ControlStatusReg = XTimerCtr_ReadReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET);

	xil_printf("Timer interrupt occurred. Count= %d\r\n", count);

	if (!paused && direction == 1) {
		count++;
	} else if (!paused && direction == 0) {
		count--;
	}
	// Auto-pause when timer reaches 0
	if (count == 0) {
		paused = true;
		direction = 1;
	}
	/*
	 * Acknowledge the interrupt by clearing the interrupt
	 * bit in the timer control status register
	 */
	XTmrCtr_WriteReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET, ControlStatusReg |XTC_CSR_INT_OCCURED_MASK);

}

void button_handler(void *callbackRef) {
	XGpio *gpio_ptr = (XGpio *) callbackRef;
	XGpio_InterruptClear(gpio_ptr, global_intr_mask);
	Xuint32 button_press = 0;
	button_press = XGpio_DiscreteRead(&sys_gpio, BUTTON_CHANNEL);
	if (button_press == 0x04) {
		// right button, reset
		count = 0;
		paused = true;
	} else if (button_press == 0x02) {
		xil_printf("left button");
	} else if (button_press == 0x10) {
		// center button, pause timer
		paused = !paused;
	} else if (button_press == 0x01) {
		// up button, increment mode
		direction = 1;
	} else if (button_press == 0x08) {
		// down button, decrement mode
		direction = 0;
	}
}

int initialize() {
	XStatus Status;
	Status = XST_SUCCESS;

	Status = XIntc_Initialize(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		if (Status == XST_DEVICE_NOT_FOUND) {
			xil_printf("XST_DEVICE_NOT_FOUND...\r\n");
		} else {
			xil_printf("a different error from XST_DEVICE_NOT_FOUND...\r\n");
		}
		xil_printf("Interrupt controller driver failed to be initialized...\r\n");
		return XST_FAILURE;
	}

	xil_printf("Interrupt controller driver initialized!\r\n");

	Status = XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR, (XInterruptHandler) timer_handler, &sys_tmrctr);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to connect the timer handler to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Connected timer to Interrupt Controller!\r\n");
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);

	Status = XTmrCtr_Initialize(&sys_tmrctr, XPAR_AXI_TIMER_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Timer initialization failed...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Initialized timer!\r\n");

	XTmrCtr_SetOptions(&sys_tmrctr, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	XTmrCtr_SetResetValue(&sys_tmrctr, 0, 0xFFFFFFFF - RESET_VALUE);
	XTmrCtr_Start(&sys_tmrctr, 0);

	microblaze_register_handler((XInterruptHandler) XIntc_DeviceInterruptHandler, (void*) XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);

	Status = XIntc_Connect(&sys_intc, XPAR_INTC_0_GPIO_0_VEC_ID, (XInterruptHandler) button_handler, &sys_gpio);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to connect the button handler to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Connected sys_gpio to Interrupt Controller!\r\n");

	Status = XGpio_Initialize(&sys_gpio, XPAR_AXI_GPIO_BTN_DEVICE_ID);
	if (Status != XST_SUCCESS) {
			xil_printf("sys_gpio initialization failed...\r\n");
			return XST_FAILURE;
	}
	xil_printf("Initialized sys_gpio!\r\n");
	XIntc_Enable(&sys_intc, XPAR_INTC_0_GPIO_0_VEC_ID);

	Status = XIntc_Start(&sys_intc, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		xil_printf("Interrupt controller driver failed to start...\r\n");
		return XST_FAILURE;
	}
	xil_printf("Started Interrupt Controller!\r\n");

	XGpio_InterruptEnable(&sys_gpio, 0x1);
	XGpio_InterruptGlobalEnable(&sys_gpio);

	microblaze_enable_interrupts();
	xil_printf("Interrupts enabled!\r\n");
}

void drawDisplay() {
	int digitOne = count % 10;
	int digitTwo = 0;
	int digitThree = 0;
	int digitFour = 0;
	int digitFive = 0;
	int digitSix = 0;
	int digitSeven = 0;
	int digitEight = 0;

	if (count > 10) {
		digitTwo = (count / 10) % 10;
	}
	if (count > 100) {
		digitThree = (count / 100) % 100;
	}
	if (count > 1000) {
		digitFour = (count / 1000) % 1000;
	}
	if (count > 10000) {
		digitFive = (count / 10000) % 10000;
	}
	if (count > 100000) {
		digitSix = (count / 100000) % 100000;
	}

//	if (tick % 8 == 0) {
//		sevenseg_draw_digit(0, digitOne);
//	} else if (tick % 8 == 1) {
//		sevenseg_draw_digit(1, digitTwo);
//	} else if (tick % 8 == 2) {
//		sevenseg_draw_digit(2, digitThree);
//	} else if (tick % 8 == 3) {
//		sevenseg_draw_digit(3, digitFour);
//	} else if (tick % 8 == 4) {
//		sevenseg_draw_digit(4, digitFive);
//	} else if (tick % 8 == 5) {
//		sevenseg_draw_digit(5, digitSix);
//	} else if (tick % 8 == 6) {
//		sevenseg_draw_digit(7, digitSeven);
//	} else if (tick % 8 == 7) {
//		sevenseg_draw_digit(7, digitEight);
//	}

	sevenseg_draw_digit(0, digitOne);
	sevenseg_draw_digit(1, digitTwo);
	sevenseg_draw_digit(2, digitThree);
	sevenseg_draw_digit(3, digitFour);
	sevenseg_draw_digit(4, digitFive);
	sevenseg_draw_digit(5, digitSix);
	sevenseg_draw_digit(6, digitSeven);
	sevenseg_draw_digit(7, digitEight);
}

int main() {
	Xil_ICacheInvalidate();
	Xil_ICacheEnable();
	Xil_DCacheInvalidate();
	Xil_DCacheEnable();

    init_platform();

	initialize();

	while (1) {
		tick++;
		drawDisplay();
	};

    cleanup_platform();
    return 0;
}
