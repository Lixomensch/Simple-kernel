#include "../include/kernel.h"
#include "../include/keyboard.h"
#include "../include/input.h"

void kmain(void *multiboot_structure, unsigned int magicnumber)
{
    clear_screen();
    kprint(" +-----------------------------------------------+\n");
    kprint("|                    JP OS                      |\n");
    kprint("+-----------------------------------------------+\n");
    kprint("\n");
    kprint("JP@kernel: ");

    while (1)
    {
        input_read();
    }
}
