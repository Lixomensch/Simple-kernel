#include "../include/kernel.h"
#include "../include/keyboard.h"
#include "../include/input.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/timer.h"
#include "../include/pmm.h"
#include "../include/multiboot.h"
#include "../include/paging.h"

void kmain(multiboot_info_t *mbd, unsigned int magicnumber)
{
    clear_screen();
    
    
    pmm_init(mbd);
    paging_init();

    pic_remap(0x20, 0x28); 
    idt_init();
    init_timer(100); 
    init_keyboard();

    kprint(" +-----------------------------------------------+\n");
    kprint("|                    JP OS                      |\n");
    kprint("+-----------------------------------------------+\n");
    kprint("\n");
    
    input_init();

    while (1)
    {
        __asm__ volatile("hlt");
    }
}
