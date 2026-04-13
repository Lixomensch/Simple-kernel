#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define KEYBOARD_STATUS_PORT 0x64

extern int cursor_x;
extern int cursor_y;
extern uint8_t terminal_color;

void move_cursor(int x, int y);
void advance_cursor();
void clear_screen();
void set_terminal_color(uint8_t color);
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
uint8_t read_key_scancode();
void reboot_system();
void put_char(char c);
void kprint(const char *str);
