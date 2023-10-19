#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

int main() {
    init_platform();

    XTmerCtr_Initialize(XTmCtr* timerController, uint TIMER_DEVICE_ID);
    

    cleanup_platform();
    return 0;
}