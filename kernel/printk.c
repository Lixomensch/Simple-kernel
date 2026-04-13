#include "printk.h"
#include "drivers/video/vga.h"
#include "arch/x86/io/io.h"

#define KEYBOARD_STATUS_PORT 0x64

void kprint(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        vga_put_char(str[i]);
    }
}

void put_char(char c)
{
    vga_put_char(c);
}

void kernel_hex_dump(uint8_t *ptr, int count)
{
    char buf[16];
    for (int i = 0; i < count; i++) {
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

void clear_screen()         { vga_clear_screen(); }
void set_terminal_color(uint8_t color) { vga_set_color(color); }
void move_cursor(int x, int y) { vga_move_cursor(x, y); }
void advance_cursor()       { vga_advance_cursor(); }

void reboot_system()
{
    outb(KEYBOARD_STATUS_PORT, 0xFE);
}
