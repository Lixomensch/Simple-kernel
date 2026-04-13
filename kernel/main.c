#include "kernel/printk.h"
#include "drivers/video/vga.h"
#include "arch/x86/cpu/idt.h"
#include "arch/x86/cpu/isr.h"
#include "arch/x86/cpu/pic.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/timer/pit.h"
#include "kernel/mm/kheap.h"
#include "kernel/mm/paging.h"
#include "kernel/mm/pmm.h"
#include "include/multiboot.h"
#include "fs/ramfs/ramfs.h"
#include "kernel/panic.h"
#include "shell/shell.h"
#include "lib/string.h"

static void page_fault_handler(registers_t *regs) {
  uint32_t faulting_address;
  __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

  kprint("\n*** PAGE FAULT ***\n");
  kprint("  Faulting address: 0x");

  char hex[16];
  for (int i = 7; i >= 0; i--) {
    uint8_t nibble = (faulting_address >> (i * 4)) & 0x0F;
    hex[7 - i] = (nibble > 9) ? ('A' + nibble - 10) : ('0' + nibble);
  }
  hex[8] = '\0';
  kprint(hex);

  kprint("\n  Error code: ");
  char buf[16];
  itoa(regs->err_code, buf);
  kprint(buf);
  kprint(" (");
  if (!(regs->err_code & 0x01)) kprint("not-present ");
  if (regs->err_code & 0x02)    kprint("write ");
  else                           kprint("read ");
  if (regs->err_code & 0x04)    kprint("user-mode");
  else                           kprint("kernel-mode");
  kprint(")\n");

  kernel_panic("Page Fault", regs);
}

void kmain(multiboot_info_t *mbd, unsigned int magicnumber) {
  vga_clear_screen();

  pmm_init(mbd);
  paging_init();
  kheap_init();

  ramfs_init();

  pic_remap(0x20, 0x28);
  idt_init();

  register_interrupt_handler(14, page_fault_handler);

  init_timer(100);
  init_keyboard();

  kprint("+-----------------------------------------------+\n");
  kprint("|                    JP OS                      |\n");
  kprint("+-----------------------------------------------+\n");

  input_init();

  while (1) {
    __asm__ volatile("hlt");
  }
}
