#include "../include/keyboard.h"
#include "../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static int shift_pressed = 0;
static int capslock_enabled = 0;

static const char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',     // 0x00 - 0x07
    '7', '8', '9', '0', '-', '=', '\b',      // 0x08 - 0x0E
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', // 0x0F - 0x16
    'i', 'o', 'p', 0, '[', '\n', 0, 'a',     // 0x17 - 0x1E
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',  // 0x1F - 0x26
    '0', '~', ']', 0, '\\', 'z', 'x', 'c',   // 0x27 - 0x2E
    'v', 'b', 'n', 'm', ',', '.', ';', 0,    // 0x2F - 0x36
    '*', 0, ' ', 0, 0, 0, 0, 0,              // 0x37 - 0x3E
};

static const char shift_map[128] = {
    0, 27, '!', '@', '#', '$', '%', 0,       // 0x00 - 0x07
    '&', '*', '(', ')', '_', '+', '\b',      // 0x08 - 0x0E
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', // 0x0F - 0x16
    'I', 'O', 'P', '`', '{', '\n', 0, 'A',   // 0x17 - 0x1E
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',  // 0x1F - 0x26
    0, '^', '}', 0, '|', 'Z', 'X', 'C',      // 0x27 - 0x2E
    'V', 'B', 'N', 'M', '<', '>', ':', 0,    // 0x2F - 0x36
    '*', 0, ' ', 0, 0, 0, 0, 0,              // 0x37 - 0x3E
};

#define SC_SHIFT_LEFT 0x2A
#define SC_SHIFT_RIGHT 0x36
#define SC_CAPSLOCK 0x3A
#define SC_ALTGR 0x38

#define KEY_RELEASE(sc) ((sc) & 0x80)

char keyboard_handler(void)
{
    uint8_t scancode = 0;
    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0)
        ;
    scancode = inb(KEYBOARD_DATA_PORT);

    if (KEY_RELEASE(scancode))
    {
        scancode &= 0x7F;
        if (scancode == SC_SHIFT_LEFT || scancode == SC_SHIFT_RIGHT)
            shift_pressed = 0;
        return 0;
    }

    if (scancode == SC_SHIFT_LEFT || scancode == SC_SHIFT_RIGHT)
    {
        shift_pressed = 1;
        return 0;
    }

    if (scancode == SC_CAPSLOCK)
    {
        capslock_enabled = !capslock_enabled;
        return 0;
    }

    char c = 0;

    if (scancode < 128)
    {
        char base = (shift_pressed ? shift_map[scancode] : scancode_map[scancode]);

        if (base >= 'a' && base <= 'z')
        {
            int upper = shift_pressed ^ capslock_enabled;
            c = upper ? (base - 32) : base;
        }
        else
        {
            c = base;
        }
    }

    return c;
}
