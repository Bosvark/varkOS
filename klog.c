#include "klog.h"
#include "uart.h"
#include "string.h"
#include "framebuffer.h"
#include "timer.h"

void output(const char *msg);
void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len);

void klogInt(const char *description, unsigned int intval)
{
/*
    char buffer[20];

    memset(buffer, 0, sizeof(buffer));

    hex_2_ascii((unsigned char*)&intval, buffer, sizeof(intval));

    uart_puts(description);
    uart_puts("0x");
    uart_puts(buffer);
    uart_puts("\r\n");
 */
	output(description);
    output(itoa((int)intval));
    output("\r\n");
}

void klogBin(const char *description, unsigned char *bindata, unsigned int binlen)
{
    char buffer[512];

    memset(buffer, 0, sizeof(buffer));

    hex_2_ascii(bindata, buffer, binlen);

    output(description);
    output("[");
    output(buffer);
    output("]\r\n");
}

void klogStr(const char *description)
{
	output(description);
    output("\r\n");
}

//
// Local functions
//
void output(const char *msg)
{
	console_write(msg);
	uart_puts(msg);
}

static const char *ascii_def={"0123456789ABCDEF"};

void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len)
{
	while(hex_len--){
        *ascii++ = *(ascii_def + ((hex[hex_len] >> 4) & 0x0f));
        *ascii++ = *(ascii_def + (hex[hex_len] & 0x0f));
    }
}

/*
void hex_2_ascii(unsigned char *hex, char *ascii, unsigned int hex_len)
{
	unsigned int pos=0;

	while(pos < hex_len){
        *ascii++ = *(ascii_def + ((hex[pos] >> 4) & 0x0f));
        *ascii++ = *(ascii_def + (hex[pos] & 0x0f));
        pos++;
    }
}
*/
