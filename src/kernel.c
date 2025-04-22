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
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    {
        VideoMemory[i] = (0x07 << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

void reboot_system()
{
    outb(KEYBOARD_STATUS_PORT, 0xFE);
}

void scroll_screen_up()
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;

    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++)
    {
        VideoMemory[i] = VideoMemory[i + VGA_WIDTH];
    }

    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        VideoMemory[i] = 0;
    }
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
            scroll_screen_up();
        }
    }
    else if (c == '\b')
    {
        if (cursor_x > 0)
        {
            cursor_x--;
        }
        else if (cursor_y > 0)
        {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        int index = cursor_y * VGA_WIDTH + cursor_x;
        VideoMemory[index] = (VideoMemory[index] & 0xFF00);
        advance_cursor();
        return;
    }
    else
    {
        int index = cursor_y * VGA_WIDTH + cursor_x;
        VideoMemory[index] = (VideoMemory[index] & 0xFF00) | c;
    }

    advance_cursor();
}

void kprint(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        put_char(str[i]);
    }
}
