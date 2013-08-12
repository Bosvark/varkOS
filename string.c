#include <stdint.h>
#include <stdarg.h>
#include "uart.h"
#include "string.h"

void *memset(void *ptr, int set, size_t size)
{
    char *p=ptr;
    
    while(size--){
        *p++ = (int)set;
    }
    
    return p;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;
    
	while (count--)
		*tmp++ = *s++;
	return dest;
}