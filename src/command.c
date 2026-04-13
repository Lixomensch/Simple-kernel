#include "../include/command.h"
#include "../include/kernel.h"
#include "../include/string.h"
#include "../include/timer.h"
#include "../include/pmm.h"

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

Command command_table[] = {
    {"help", command_help},     {"clear", command_clear},
    {"echo", command_echo},     {"reboot", command_reboot},
    {"uptime", command_uptime}, {"color", command_color},
    {"meminfo", command_meminfo},
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
