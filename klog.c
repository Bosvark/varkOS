#include "klog.h"
#include "uart.h"
#include "string.h"

void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len);

void klogInt(const char *description, unsigned int intval)
{
    char buffer[9];
    unsigned char intbuff[20];
    (void)description;
    memset(buffer, 0, sizeof(buffer));
    memset(intbuff, 0, sizeof(intbuff));
    memcpy((unsigned char*)intbuff, &intval, sizeof(intval));
    
    hex_2_ascii(intbuff, buffer, sizeof(intbuff));
    
    uart_puts(description);
    uart_puts("0x");
    uart_puts(buffer);
    uart_puts("\r\n");
}

const char *ascii_def={"0123456789ABCDEF"};

void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len)
{
    uint32_t i,pos=0;
    
    for(i=0; i<hex_len; i++){
        ascii[pos++] = ascii_def[(hex[i] >> 4)&0x0f];
        ascii[pos++] = ascii_def[hex[i]&0x0f];
    }
}
