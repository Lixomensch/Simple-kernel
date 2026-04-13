#include "command.h"
#include "kernel/printk.h"
#include "lib/string.h"
#include "drivers/timer/pit.h"
#include "kernel/mm/pmm.h"
#include "fs/vfs.h"

typedef void (*CommandFunction)(const char *args);

typedef struct {
  const char *name;
  CommandFunction function;
} Command;

void command_help(const char *args) {
  kprint("Comandos disponiveis:\n");
  kprint(" - help   : mostra esta ajuda\n");
  kprint(" - clear  : limpa a tela\n");
  kprint(" - echo   : repete o texto\n");
  kprint(" - uptime : mostra tempo ligado\n");
  kprint(" - color  : muda a cor do texto\n");
  kprint(" - meminfo: mostra memoria disponivel\n");
  kprint(" - touch  : cria um arquivo vazio\n");
  kprint(" - write  : escreve em um arquivo (ex: write a.txt texto)\n");
  kprint(" - ls     : lista arquivos\n");
  kprint(" - cat    : le o conteudo de um arquivo\n");
  kprint(" - reboot : reinicia o sistema\n");
}

void command_clear(const char *args) { clear_screen(); }

void command_echo(const char *args) {
  if (args && *args) {
    kprint(args);
  }
  kprint("\n");
}

void command_reboot(const char *args) { reboot_system(); }

void command_uptime(const char *args) {
  uint32_t uptime = get_uptime_seconds();
  char buffer[32];
  itoa(uptime, buffer);
  kprint("Uptime: ");
  kprint(buffer);
  kprint(" segundos\n");
}

void command_color(const char *args) {
  if (args[0] == '0')
    set_terminal_color(0x07);
  else if (args[0] == '1')
    set_terminal_color(0x09);
  else if (args[0] == '2')
    set_terminal_color(0x0A);
  else if (args[0] == '3')
    set_terminal_color(0x0C);
  else if (args[0] == '4')
    set_terminal_color(0x0E);
  else {
    kprint("Cores:\n 0=Branco\n 1=Azul\n 2=Verde\n 3=Vermelho\n 4=Amarelo\n");
  }
}

void command_meminfo(const char *args) {
  uint32_t free_mem = pmm_get_free_memory();
  char buffer[32];
  itoa(free_mem / 1024, buffer);
  kprint("Memoria Livre: ");
  kprint(buffer);
  kprint(" KB\n");
}

void command_touch(const char *args) {
  if (!fs_root) return;
  if (!args || args[0] == '\0') {
    kprint("Uso: touch <arquivo>\n");
    return;
  }

  char filename[128];
  int i = 0;
  while(args[i] && args[i] != ' ' && i < 127) {
    filename[i] = args[i];
    i++;
  }
  filename[i] = '\0';

  if (vfs_create(fs_root, filename)) {
    kprint("Arquivo criado: ");
    kprint(filename);
    kprint("\n");
  } else {
    kprint("Falha ao criar arquivo (ou ja existe).\n");
  }
}

void command_write(const char *args) {
  if (!fs_root) return;

  char filename[128];
  int i = 0;
  while(args[i] && args[i] != ' ' && i < 127) {
    filename[i] = args[i];
    i++;
  }
  filename[i] = '\0';

  if (filename[0] == '\0') {
    kprint("Uso: write <arquivo> <texto...>\n");
    return;
  }

  fs_node_t *node = vfs_find(fs_root, filename);
  if (!node) {
    kprint("Erro: Arquivo nao encontrado. Use 'touch' primeiro.\n");
    return;
  }

  const char *text = args + i;
  while(*text == ' ') text++;

  uint32_t len = strlen(text);

  uint32_t written = vfs_write(node, text, len);
  if (written > 0) {
    kprint("Escritos ");
    char num[16];
    itoa(written, num);
    kprint(num);
    kprint(" bytes com sucesso.\n");
  } else {
    kprint("Falha ao escrever (arquivo cheio ou erro no VFS).\n");
  }
}

void command_ls(const char *args) {
  if (!fs_root) {
    kprint("Nenhum sistema de arquivos montado.\n");
    return;
  }
  vfs_list(fs_root);
}

void command_cat(const char *args) {
  if (!fs_root) return;
  if (!args || args[0] == '\0') {
    kprint("Uso: cat <arquivo>\n");
    return;
  }

  char filename[128];
  int i=0;
  while(args[i] && args[i] != ' ' && i < 127) {
    filename[i] = args[i];
    i++;
  }
  filename[i] = '\0';

  fs_node_t *fsnode = vfs_find(fs_root, filename);
  if (!fsnode) {
    kprint("Arquivo nao encontrado.\n");
    return;
  }

  if ((fsnode->flags & 0x07) == FS_DIRECTORY) {
    kprint("Erro: e um diretorio.\n");
    return;
  }

  char buffer[512];
  uint32_t sz = vfs_read(fsnode, buffer, 511);
  if (sz > 0) {
      buffer[sz] = '\0';
      kprint(buffer);
  }
  kprint("\n");
}

Command command_table[] = {
    {"help", command_help},     {"clear", command_clear},
    {"echo", command_echo},     {"reboot", command_reboot},
    {"uptime", command_uptime}, {"color", command_color},
    {"meminfo", command_meminfo}, {"ls", command_ls},
    {"cat", command_cat},       {"touch", command_touch},
    {"write", command_write},
};

const int command_count = sizeof(command_table) / sizeof(Command);

void execute_command(const char *input) {
  if (input[0] == '\0')
    return;

  char command_buffer[128];
  int i = 0;
  while (input[i] && input[i] != ' ' && i < (int)(sizeof(command_buffer) - 1)) {
    command_buffer[i] = input[i];
    i++;
  }
  command_buffer[i] = '\0';

  const char *args = input + i;
  while (*args == ' ')
    args++;

  for (int j = 0; j < command_count; j++) {
    if (strcmp(command_buffer, command_table[j].name) == 0) {
      command_table[j].function(args);
      return;
    }
  }

  kprint("Comando desconhecido: ");
  kprint(command_buffer);
  kprint("\n");
}
