#include "../include/input.h"
#include "../include/kernel.h"
#include "../include/keyboard.h"
#include "../include/command.h"

static char input_buffer[128];
static int input_index = 0;

void input_init()
{
    kprint("JP@kernel: ");
    input_index = 0;
}

void input_read()
{
    char c = keyboard_handler();
    if (!c)
        return;

    if ((uint8_t)c == 27)
    {
        kprint("\nReiniciando o sistema...\n");
        reboot_system();
    }
    else if (c == '\n')
    {
        input_buffer[input_index] = '\0';
        kprint("\n");

        execute_command(input_buffer);

        input_init();
    }
    else if (c == '\b')
    {
        if (input_index > 0)
        {
            input_index--;
            input_buffer[input_index] = '\0';
            put_char(c);
        }
    }
    else
    {
        if (input_index < 128 - 1)
        {
            input_buffer[input_index++] = c;
            put_char(c);
        }
    }
}
