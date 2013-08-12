/* main.c - the entry point for the kernel */

#include <stdint.h>
#include "uart.h"
#include "timer.h"
//#include "framebuffer.h"
#include "string.h"
#include "klog.h"

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
    uint32_t count=12345;
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    uart_init();
    
    uart_puts("\r\n*** varkOS ***\r\n");

//    FramebufferInit(&fb_info);
    
//    fb_test();

    while(1)
    {
        unsigned int intval=0x31323334;
        klogInt("number 1:", intval);
        waitUS(TIMER_ONE_SECOND);

        klogInt("Counter:", count++);
        waitUS(TIMER_ONE_SECOND);
    }
 }
