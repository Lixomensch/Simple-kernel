#include "pit.h"
#include "arch/x86/cpu/isr.h"
#include "arch/x86/io/io.h"

static uint32_t tick = 0;
static uint32_t timer_frequency = 0;

static void timer_callback(registers_t *regs) {
    tick++;
}

void init_timer(uint32_t frequency) {
    timer_frequency = frequency;

    register_interrupt_handler(32, timer_callback);

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

uint32_t get_uptime_seconds() {
    if (timer_frequency == 0) return 0;
    return tick / timer_frequency;
}
