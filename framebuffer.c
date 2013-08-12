#include <stdint.h>
#include "framebuffer.h"
#include "mailbox.h"
#include "uart.h"

uint32_t FramebufferInit(FramebufferInfo *fb_info)
{
    uint32_t retval=0;
    
    MailboxWrite((uint32_t)(fb_info+0x40000000), 1);
    
    if((retval = MailboxRead(1)) == 0){
        uart_puts("Framebuffer init failed\r\n");
        return 0;
    }
    else{
        uart_puts("Framebuffer initialised\r\n");
        fb_info=(FramebufferInfo*)retval;
        return 1;
    }
}