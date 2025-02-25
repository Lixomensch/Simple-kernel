#include "kernel.h"

void kmain(void *multiboot_structure, unsigned int magicnumber)
{
    printf("  +-----------------------------------------------+\n");
    printf("  |                  ::JP OS::                    |\n");
    printf("  +-----------------------------------------------+\n");
    printf("  |        PRESSIONE [ESC] PARA REINICIAR         |\n");
    printf("  +-----------------------------------------------+\n");

    while (1)
    {
        uint8_t scancode = read_key_scancode();

        // Verifica se o código de varredura corresponde à tecla ESC para reiniciar o sistema
        if (scancode == ESC_SCANCODE)
        {
            printf("\nReiniciando o sistema...\n");
            reboot_system();
        }
        else
        {
            char character;

            // Loop de digitação
            while (1)
            {
                scancode = read_key_scancode();

                // Verifica teclas A-Z
                if (scancode >= 0x1E && scancode <= 0x36)
                {
                    character = 'a' + (scancode - 0x1E);
                    put_char(character);
                }
                // Verifica teclas 0-9
                else if (scancode >= 0x10 && scancode <= 0x19)
                {
                    character = '0' + (scancode - 0x10);
                    put_char(character);
                }
                // Verifica a tecla Enter
                else if (scancode == 0x1C)
                {
                    put_char('\n');
                }
                // Verifica a tecla Espaço
                else if (scancode == 0x39)
                {
                    put_char(' ');
                }
                // Verifica a tecla Backspace
                else if (scancode == 0x0E)
                {
                    if (cursor_x > 0) // Verifica se não está no início da linha
                    {
                        cursor_x--;
                        put_char(' ');
                        cursor_x--;
                    }
                    else if (cursor_y > 0) // Verifica se não está na primeira linha
                    {
                        cursor_y--;
                        cursor_x = VGA_WIDTH - 1; // Move para o final da linha anterior
                        put_char(' ');
                        cursor_x = VGA_WIDTH - 1;
                    }
                }

                // Condição para sair do loop interno
                if (scancode == ESC_SCANCODE)
                    break;
            }
        }
    }
}
