#pragma once

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define ESC_SCANCODE 0x01
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static int cursor_x = 0, cursor_y = 0;

void move_cursor(int x, int y);
void advance_cursor();
void clear_screen();

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

uint8_t read_key_scancode();
void reboot_system();

void printf(const char *str);
void put_char(char c);

void move_cursor(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
}

void advance_cursor()
{
    cursor_x++;
    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= VGA_HEIGHT)
    {
        cursor_y = VGA_HEIGHT - 1;
    }
}

void clear_screen()
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | ' ';
    }
}

uint8_t inb(uint16_t port)
{
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t read_key_scancode()
{
    uint8_t scancode = 0;
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
        ;
    scancode = inb(KEYBOARD_DATA_PORT);
    return scancode;
}

void reboot_system()
{
    outb(KEYBOARD_STATUS_PORT, 0xFE);
}

void put_char(char c)
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;

    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT)
        {
            cursor_y = VGA_HEIGHT - 1;
        }
    }
    else
    {
        int index = cursor_y * VGA_WIDTH + cursor_x;
        VideoMemory[index] = (VideoMemory[index] & 0xFF00) | c;
        advance_cursor();
    }
}

void printf(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        put_char(str[i]);
    }
}
