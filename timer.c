#include <stdint.h>
#include "mmio.h"
#include "timer.h"

enum{
    TIMER_BASE_ADDR =0x20003000,
    CONTROL_STATUS  =(TIMER_BASE_ADDR+4),
    COUNTER         =(TIMER_BASE_ADDR+12),
    COMPARE0        =(TIMER_BASE_ADDR+16),
    COMPARE1        =(TIMER_BASE_ADDR+20),
    COMPARE2        =(TIMER_BASE_ADDR+24),
    COMPARE3        =(TIMER_BASE_ADDR+28),
};

#define get_timestamp() mmio_read(COUNTER)

void waitUS(uint32_t us)
{
    uint32_t wait_time = get_timestamp();
    wait_time += us;
    while(get_timestamp() < wait_time);
}
