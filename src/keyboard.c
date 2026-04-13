#include "../include/keyboard.h"
#include "../include/io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static int shift_pressed = 0;
static int capslock_enabled = 0;

static const char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '\b',      
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 
    'i', 'o', 'p', 0, '[', '\n', 0, 'a',     
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',  
    '0', '~', ']', 0, '\\', 'z', 'x', 'c',   
    'v', 'b', 'n', 'm', ',', '.', ';', 0,    
    '*', 0, ' ', 0, 0, 0, 0, 0,              
};

static const char shift_map[128] = {
    0, 27, '!', '@', '#', '$', '%', 0,       
    '&', '*', '(', ')', '_', '+', '\b',      
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 
    'I', 'O', 'P', '`', '{', '\n', 0, 'A',   
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',  
    0, '^', '}', 0, '|', 'Z', 'X', 'C',      
    'V', 'B', 'N', 'M', '<', '>', ':', 0,    
    '*', 0, ' ', 0, 0, 0, 0, 0,              
};

#define SC_SHIFT_LEFT 0x2A
#define SC_SHIFT_RIGHT 0x36
#define SC_CAPSLOCK 0x3A

#define KEY_RELEASE(sc) ((sc) & 0x80)

void keyboard_handler(registers_t *regs)
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (KEY_RELEASE(scancode))
    {
        scancode &= 0x7F;
        if (scancode == SC_SHIFT_LEFT || scancode == SC_SHIFT_RIGHT)
            shift_pressed = 0;
        return;
    }

    if (scancode == SC_SHIFT_LEFT || scancode == SC_SHIFT_RIGHT)
    {
        shift_pressed = 1;
        return;
    }

    if (scancode == SC_CAPSLOCK)
    {
        capslock_enabled = !capslock_enabled;
        return;
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

    if (c) {
        
        extern void input_handle_key(char c);
        input_handle_key(c);
    }
}

void init_keyboard() {
    register_interrupt_handler(33, keyboard_handler);
}
