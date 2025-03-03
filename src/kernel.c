#include "../include/kernel.h"

int cursor_x = 0;
int cursor_y = 0;

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
    volatile uint16_t *VideoMemory = VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    {
        VideoMemory[i] = (0x07 << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
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

void kprint(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        put_char(str[i]);
    }
}
