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

void ptr_test(unsigned char *input, unsigned char *output, unsigned int length)
{
	while(length--)
		*output++ = *input++;
}
/*
void ptr_test(unsigned char *input, unsigned char *output, unsigned int length)
{
	int pos=0;
	do{
		output[pos] = input[pos];
		pos++;
	}while(pos < length);
}

void ptr_test(unsigned char *input, unsigned char *output, unsigned int length)
{
	memcpy(output, input, length);
}
*/
void run_test(void)
{
	int val1=0x12345678,val2=0;

	ptr_test((unsigned char*)&val1,(unsigned char*)&val2, sizeof(val1));

	if(val1 == val2){
		gpioWrite(PIN_LED_OK, 0);
	}

	while(1); waitUS(TIMER_HALF_SECOND);
}
// kernel main function, it all begins here
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
//	unsigned int intval=0x31323334;

    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    uart_init();
    
    gpioSetOutput(PIN_LED_OK);

    run_test();

    uart_puts("\r\n*** varkOS ***\r\n");

//    FramebufferInit(&fb_info);
    
//    fb_test();

    while(1)
    {
//    	gpioToggle(PIN_LED_OK);
//    	gpioWriteSafe(PIN_LED_OK, 0);

//        klogInt("klogInt:", intval);
//        waitUS(TIMER_HALF_SECOND);

//        gpioToggle(PIN_LED_OK);
//       gpioWrite(PIN_LED_OK, 1);

//        klogBin("klogBin:", (unsigned char*)"\x12\x34\x56\x78", 4);
//        waitUS(TIMER_HALF_SECOND);
    }
 }
