/* main.c - the entry point for the kernel */

#include <stdint.h>
#include <stdint.h>
#include "uart.h"
#include "timer.h"
#include "framebuffer.h"
#include "string.h"
#include "klog.h"
#include "gpio.h"

#define UNUSED(x) (void)(x)

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    uart_init();
    uart_puts("\r\n*** varkOS ***\r\n");

    FramebufferInit();

    gpioSetOutput(PIN_LED_OK);

    while(1)
    {
    	gpioToggle(PIN_LED_OK);
        waitUS(TIMER_HALF_SECOND);
    }
 }
