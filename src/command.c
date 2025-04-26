#include "../include/command.h"
#include "../include/kernel.h"
#include "../include/string.h"

typedef void (*CommandFunction)(const char *args);

typedef struct
{
    const char *name;
    CommandFunction function;
} Command;

// === Funções dos comandos ===

void command_help(const char *args)
{
    kprint("Comandos disponiveis:\n");
    kprint(" - help   : mostra esta ajuda\n");
    kprint(" - clear  : limpa a tela\n");
    kprint(" - echo   : repete o texto\n");
    kprint(" - reboot : reinicia o sistema\n");
}

void command_clear(const char *args)
{
    clear_screen();
}

void command_echo(const char *args)
{
    if (args && *args)
    {
        kprint(args);
    }
    kprint("\n");
}

void command_reboot(const char *args)
{
    reboot_system();
}

// === Tabela de comandos ===

Command command_table[] = {
    {"help", command_help},
    {"clear", command_clear},
    {"echo", command_echo},
    {"reboot", command_reboot},
};

const int command_count = sizeof(command_table) / sizeof(Command);

// === Execução do comando ===

void execute_command(const char *input)
{
    if (input[0] == '\0')
        return;

    char command_buffer[128];
    int i = 0;
    while (input[i] && input[i] != ' ' && i < (int)(sizeof(command_buffer) - 1))
    {
        command_buffer[i] = input[i];
        i++;
    }
    command_buffer[i] = '\0';

    const char *args = input + i;
    while (*args == ' ')
        args++;

    for (int j = 0; j < command_count; j++)
    {
        if (strcmp(command_buffer, command_table[j].name) == 0)
        {
            command_table[j].function(args);
            return;
        }
    }

    kprint("Comando desconhecido: ");
    kprint(command_buffer);
    kprint("\n");
}
