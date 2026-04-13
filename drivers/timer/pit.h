#ifndef DRIVERS_TIMER_PIT_H
#define DRIVERS_TIMER_PIT_H

#include <stdint.h>

void init_timer(uint32_t frequency);
uint32_t get_uptime_seconds();

#endif
