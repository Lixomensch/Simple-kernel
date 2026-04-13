#ifndef KERNEL_PRINTK_H
#define KERNEL_PRINTK_H

#include <stdint.h>

void kprint(const char *str);
void put_char(char c);
void kernel_hex_dump(uint8_t *ptr, int count);

void clear_screen();
void set_terminal_color(uint8_t color);
void move_cursor(int x, int y);
void advance_cursor();
void reboot_system();

#endif
