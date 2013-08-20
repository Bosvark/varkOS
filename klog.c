#include "klog.h"
#include "uart.h"
#include "string.h"
#include "timer.h"

void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len);

void klogInt(const char *description, unsigned int intval)
{
    char buffer[20];

    memset(buffer, 0, sizeof(buffer));

    hex_2_ascii((unsigned char*)&intval, buffer, sizeof(intval));

    uart_puts(description);
    uart_puts("0x");
    uart_puts(buffer);
    uart_puts("\r\n");
}

void klogBin(const char *description, unsigned char *bindata, unsigned int binlen)
{
    char buffer[512];

    memset(buffer, 0, sizeof(buffer));

    hex_2_ascii(bindata, buffer, binlen);

    uart_puts(description);
    uart_puts("[");
    uart_puts(buffer);
    uart_puts("]\r\n");
}

//
// Local functions
//

static const char *ascii_def={"0123456789ABCDEF"};

void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len)
{
	while(hex_len--){
        *ascii++ = *(ascii_def + ((hex[hex_len] >> 4) & 0x0f));
        *ascii++ = *(ascii_def + (hex[hex_len] & 0x0f));
    }
}

