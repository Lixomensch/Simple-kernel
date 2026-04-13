#ifndef ARCH_X86_CPU_PIC_H
#define ARCH_X86_CPU_PIC_H

#include <stdint.h>

void pic_remap(int offset1, int offset2);
void pic_send_eoi(unsigned char irq);

#endif
