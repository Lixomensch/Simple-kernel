#pragma once

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t *)0xB8000)
#define ESC_SCANCODE 0x01
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

extern int cursor_x;
extern int cursor_y;

void move_cursor(int x, int y);
void advance_cursor();
void clear_screen();
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
uint8_t read_key_scancode();
void reboot_system();
void put_char(char c);
void kprint(const char *str);
