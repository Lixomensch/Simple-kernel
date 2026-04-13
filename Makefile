# ==================================================================
# Simple-kernel Makefile
# ==================================================================
CC      = gcc
AS      = as
LD      = ld

# === Flags ===
DEBUG   = -g
OPT     = -O1
WARN    = -Wall -Werror
CFLAGS  = $(DEBUG) $(OPT) $(WARN) -m32 -ffreestanding -fno-builtin \
          -fno-exceptions -fno-stack-protector \
          -I.
ASFLAGS = --32
LDFLAGS = -melf_i386

# === Directories ===
BUILD   = build
TARGET  = $(BUILD)/kernel.bin
LINKER  = arch/x86/boot/linker.ld

# ==================================================================
# Sources by module
# ==================================================================

# Architecture (x86)
ARCH_S  = arch/x86/boot/loader.s \
          arch/x86/cpu/interrupts.s
ARCH_C  = arch/x86/cpu/idt.c \
          arch/x86/cpu/isr.c \
          arch/x86/cpu/pic.c \
          arch/x86/io/io.c

# Kernel core
KERNEL_C = kernel/main.c \
           kernel/panic.c \
           kernel/printk.c \
           kernel/mm/pmm.c \
           kernel/mm/paging.c \
           kernel/mm/kheap.c

# Drivers
DRIVER_C = drivers/video/vga.c \
           drivers/keyboard/keyboard.c \
           drivers/timer/pit.c \
           drivers/ata/ata.c

# Filesystem
FS_C    = fs/vfs.c \
          fs/ramfs/ramfs.c

# Shell
SHELL_C = shell/shell.c \
          shell/command.c

# Library
LIB_C   = lib/string.c

# ==================================================================
# Object files
# ==================================================================
ALL_C   = $(ARCH_C) $(KERNEL_C) $(DRIVER_C) $(FS_C) $(SHELL_C) $(LIB_C)
ALL_S   = $(ARCH_S)
C_OBJS  = $(ALL_C:%.c=$(BUILD)/%.o)
S_OBJS  = $(ALL_S:%.s=$(BUILD)/%.o)
OBJS    = $(C_OBJS) $(S_OBJS)

# ==================================================================
# Build rules
# ==================================================================
all: $(TARGET)

$(TARGET): $(LINKER) $(OBJS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -T $< -o $@ $(OBJS)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# ==================================================================
# Disk and Run
# ==================================================================
$(BUILD)/disk.img:
	@mkdir -p $(BUILD)
	dd if=/dev/zero of=$@ bs=1M count=1
	printf "MENSAGEM SECRETA NO HD VIA SETOR ZERO ATA!!" | dd of=$@ conv=notrunc

run: $(TARGET) $(BUILD)/disk.img
	qemu-system-i386 -kernel $(TARGET) -drive file=$(BUILD)/disk.img,format=raw,if=ide

clean:
	rm -rf $(BUILD)

.PHONY: all clean run
