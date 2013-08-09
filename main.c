/* main.c - the entry point for the kernel */

#include <stdint.h>
#include "uart.h"
#include "timer.h"

#define UNUSED(x) (void)(x)

const char hello[] = "\r\nHello World\r\n";

extern uint32_t waitTime(uint32_t us);

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    uart_init();
    
    uart_puts("*** varkOS ***");
    
    while(1){
        uart_puts(hello);
        waitUS(1000000);
    }
 }
