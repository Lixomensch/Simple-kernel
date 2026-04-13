#ifndef DRIVERS_VIDEO_VGA_H
#define DRIVERS_VIDEO_VGA_H

#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

extern int cursor_x;
extern int cursor_y;
extern uint8_t terminal_color;

void vga_move_cursor(int x, int y);
void vga_advance_cursor();
void vga_clear_screen();
void vga_set_color(uint8_t color);
void vga_put_char(char c);
void vga_scroll_up();
void vga_update_hardware_cursor();

#endif
