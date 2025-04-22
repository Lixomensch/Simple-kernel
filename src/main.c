#include "../include/kernel.h"
#include "../include/keyboard.h"

void kmain(void *multiboot_structure, unsigned int magicnumber)
{
    clear_screen();
    kprint(" +-----------------------------------------------+\n");
    kprint("|                    JP OS                      |\n");
    kprint("+-----------------------------------------------+\n");
    kprint("|        PRESSIONE [ESC] PARA REINICIAR         |\n");
    kprint("+-----------------------------------------------+\n");

    while (1)
    {
        char c = keyboard_handler();
        if (!c)
            continue;

        if ((uint8_t)c == 27)
        {
            kprint("\nReiniciando o sistema...\n");
            reboot_system();
        }
        else
        {
            put_char(c);
        }
    }
}
