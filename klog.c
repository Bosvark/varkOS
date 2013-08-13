#include "klog.h"
#include "uart.h"
#include "string.h"
#include "timer.h"

void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len);
void dbg(const char *str)
{
	while(*str){
		uart_putc(*str++);
		waitUS(100000);
	}
}
void klogInt(const char *description, unsigned int intval)
{
    char buffer[20];

    memset(buffer, 0, sizeof(buffer));
intval=0;
//    hex_2_ascii((unsigned char*)&test, buffer, sizeof(intval));
hex_2_ascii((unsigned char*)"\x31\x32\x33\x34", buffer, sizeof(intval));
(void)description;
/*
    dbg(description);
    dbg("0x");
//    uart_puts(buffer);
    dbg(buffer);
*/    dbg("\r\n");

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
	int pos=0;

	while(pos < hex_len){
uart_putc(hex[pos]);
        *ascii++ = *(ascii_def + ((hex[pos] >> 4) & 0x0f));
        *ascii++ = *(ascii_def + (hex[pos] & 0x0f));
        pos++;
    }
}

