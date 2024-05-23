#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

void main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Missing ROM name.\n");
        exit(EXIT_FAILURE);
    }

    Screen screen = create_screen();
    init_screen(&screen);

    Cpu cpu = create_cpu(&screen);
    load_rom(&cpu, argv[1]);
    draw_screen(&cpu);

    char input[25] = "";
    while (strcmp(input, "exit") != 0)
    {
        wrefresh(screen.input);
        mvwgetstr(screen.input, 1, 1, input);
        werase(screen.input);
        box(screen.input, 0, 0);
        if (strcmp(input, "s") == 0)
            step(&cpu);
        draw_screen(&cpu);
    }
}