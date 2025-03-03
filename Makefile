# ==================================================================
# Configurações do compilador e flags
# ==================================================================
CC = gcc
AS = as
LD = ld
TARGET = build/kernel.bin

# Flags de compilação
DEBUG = -g
OPT = -O1
WARN = -Wall -Werror
CFLAGS = $(DEBUG) $(OPT) $(WARN) -m32 -ffreestanding -fno-builtin -fno-exceptions
ASFLAGS = --32
LDFLAGS = -melf_i386

# ==================================================================
# Caminhos e diretórios
# ==================================================================
BUILD_DIR = build
SRC_DIR = src

# ==================================================================
# Arquivos de fontes e objetos
# ==================================================================
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
S_SRCS = $(wildcard $(SRC_DIR)/*.s)

# Criar variáveis de objetos, com a pasta de objetos
C_OBJS = $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
S_OBJS = $(S_SRCS:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)
OBJS = $(C_OBJS) $(S_OBJS)

# ==================================================================
# Compilação de arquivos Assembly
# ==================================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# ==================================================================
# Compilação de arquivos C
# ==================================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ==================================================================
# Linkagem
# ==================================================================
$(TARGET): linker.ld $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T $< -o $@ $(OBJS)

# ==================================================================
# Instalação
# ==================================================================
install: $(TARGET)
	sudo cp $(TARGET) /boot/kernel.bin

# ==================================================================
# Limpeza
# ==================================================================
clean:
	@rm -rf $(BUILD_DIR) $(TARGET)

# ==================================================================
# Regras especiais
# ==================================================================
.PHONY: clean install
