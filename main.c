/* main.c - the entry point for the kernel */

#include <stdint.h>
#include <stdint.h>
#include "uart.h"
#include "timer.h"
//#include "framebuffer.h"
#include "string.h"
#include "klog.h"
#include "gpio.h"

#define UNUSED(x) (void)(x)

//static FramebufferInfo fb_info={1920,1080,1920,1080,0,32,0,0,0,0};
/*
void fb_test(void)
{
    uint32_t x=0,y=0,color=0;
    uint32_t *ptr=fb_info.gpu_ptr;
    
    for(x=0; x<fb_info.physical_width; x++){
        color=0;
        
        for(y=0;y<fb_info.physical_height;y++){
            *ptr=color;
            color++;
            ptr++;
        }
    }
}
*/

// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    uart_init();
    uart_puts("\r\n*** varkOS ***\r\n");

//    FramebufferInit(&fb_info);
//    fb_test();

    gpioSetOutput(PIN_LED_OK);

    while(1)
    {
    	int val=0x12345678;
        klogInt("klogInt:", val);
    	gpioToggle(PIN_LED_OK);

        waitUS(TIMER_HALF_SECOND);

        gpioToggle(PIN_LED_OK);

        klogBin("klogBin:", (unsigned char*)"\x12\x34\x56\x78", 4);
        waitUS(TIMER_HALF_SECOND);
    }
 }
