#include "vga.h"
#include "arch/x86/io/io.h"

int cursor_x = 0;
int cursor_y = 0;
uint8_t terminal_color = 0x07;

void vga_set_color(uint8_t color) {
    terminal_color = color;
}

void vga_update_hardware_cursor()
{
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_move_cursor(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
    vga_update_hardware_cursor();
}

void vga_scroll_up()
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;

    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++)
    {
        VideoMemory[i] = VideoMemory[i + VGA_WIDTH];
    }

    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
    {
        VideoMemory[i] = ((uint16_t)terminal_color << 8) | ' ';
    }
}

void vga_advance_cursor()
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
        vga_scroll_up();
    }
    vga_update_hardware_cursor();
}

void vga_clear_screen()
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    {
        VideoMemory[i] = ((uint16_t)terminal_color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_hardware_cursor();
}

void vga_put_char(char c)
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;

    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT)
        {
            cursor_y = VGA_HEIGHT - 1;
            vga_scroll_up();
        }
        vga_update_hardware_cursor();
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
        VideoMemory[index] = ((uint16_t)terminal_color << 8) | ' ';
        vga_update_hardware_cursor();
    }
    else
    {
        int index = cursor_y * VGA_WIDTH + cursor_x;
        VideoMemory[index] = ((uint16_t)terminal_color << 8) | c;
        vga_advance_cursor();
    }
}
