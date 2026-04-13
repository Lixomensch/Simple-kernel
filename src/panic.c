#include "../include/panic.h"
#include "../include/kernel.h"
#include "../include/string.h"

void kernel_panic(const char* message, registers_t* regs) {
    
    set_terminal_color(0x4F);
    clear_screen();

    kprint("\n\n  *** KERNEL PANIC ***\n\n");
    kprint("  Message: ");
    kprint(message);
    kprint("\n\n");

    if (regs) {
        char buf[32];
        
        kprint("  Exception (int_no): ");
        itoa(regs->int_no, buf);
        kprint(buf);
        kprint("\n  Error Code: ");
        itoa(regs->err_code, buf);
        kprint(buf);
        kprint("\n\n  --- REGISTERS ---\n");

        kprint("  EAX: "); itoa(regs->eax, buf); kprint(buf);
        kprint("  EBX: "); itoa(regs->ebx, buf); kprint(buf);
        kprint("  ECX: "); itoa(regs->ecx, buf); kprint(buf);
        kprint("  EDX: "); itoa(regs->edx, buf); kprint(buf);
        kprint("\n");
        kprint("  ESI: "); itoa(regs->esi, buf); kprint(buf);
        kprint("  EDI: "); itoa(regs->edi, buf); kprint(buf);
        kprint("  EBP: "); itoa(regs->ebp, buf); kprint(buf);
        kprint("  ESP: "); itoa(regs->esp, buf); kprint(buf);
        kprint("\n");
        kprint("  EIP: "); itoa(regs->eip, buf); kprint(buf);
        kprint("  CS:  "); itoa(regs->cs, buf); kprint(buf);
        kprint("  EFLAGS: "); itoa(regs->eflags, buf); kprint(buf);
    }

    kprint("\n\n  System Halted.\n");

    while (1) {
        __asm__ volatile("cli; hlt");
    }
}
