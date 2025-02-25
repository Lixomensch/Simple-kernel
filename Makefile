# Configurações do compilador
CC = gcc
LD = i686-elf-ld
AS = nasm
CFLAGS = -ffreestanding -m32 -nostdlib -lgcc -Wall -Wextra
ASFLAGS = -f elf
LDFLAGS = -T linker.ld

# Diretórios
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Arquivos-fonte
SRCS = $(wildcard $(SRC_DIR)/kernel/*.c)

# Arquivos-objeto
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Nome do kernel binário final
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# Nome da ISO gerada
ISO_NAME = kernel.iso

.PHONY: all clean run iso

# Alvo padrão: compilar tudo
all: $(KERNEL_BIN)

# Compilar arquivos .c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Linkar o kernel
$(KERNEL_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Criar a ISO
iso: $(KERNEL_BIN)
	@mkdir -p iso/boot
	@cp $(KERNEL_BIN) iso/boot/
	grub-mkrescue -o $(ISO_NAME) iso/

# Limpar arquivos compilados
clean:
	rm -rf $(BUILD_DIR) $(ISO_NAME) iso/boot/kernel.bin

# Rodar no QEMU
run: iso
	qemu-system-x86_64 -cdrom $(ISO_NAME)
