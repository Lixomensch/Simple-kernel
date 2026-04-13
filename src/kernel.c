#include "../include/kernel.h"

void scroll_screen_up();

int cursor_x = 0;
int cursor_y = 0;
uint8_t terminal_color = 0x07;

void set_terminal_color(uint8_t color) {
    terminal_color = color;
}

void kernel_hex_dump(uint8_t* ptr, int count) {
    char buf[16];
    for (int i=0; i<count; i++) {
        uint8_t val = ptr[i];
        
        
        uint8_t h = val >> 4;
        buf[0] = (h > 9) ? (h - 10) + 'A' : h + '0';
        
        uint8_t l = val & 0x0F;
        buf[1] = (l > 9) ? (l - 10) + 'A' : l + '0';
        buf[2] = ' ';
        buf[3] = '\0';
        kprint(buf);
        
        if ((i + 1) % 16 == 0) kprint("\n");
    }
}

void update_hardware_cursor()
{
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void move_cursor(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
    update_hardware_cursor();
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
        scroll_screen_up();
    }
    update_hardware_cursor();
}

void clear_screen()
{
    unsigned short *VideoMemory = (unsigned short *)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    {
        VideoMemory[i] = ((uint16_t)terminal_color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    update_hardware_cursor();
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
        VideoMemory[i] = ((uint16_t)terminal_color << 8) | ' ';
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
        update_hardware_cursor();
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
        update_hardware_cursor();
    }
    else
    {
        int index = cursor_y * VGA_WIDTH + cursor_x;
        VideoMemory[index] = ((uint16_t)terminal_color << 8) | c;
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
