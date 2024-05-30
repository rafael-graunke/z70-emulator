#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "cpu.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Missing ROM name.\n");
        return EXIT_FAILURE;
    }

    Screen *screen = create_screen();
    Cpu cpu = create_cpu(screen);
    load_rom(&cpu, argv[1]);
    draw_screen(&cpu);

    char input[50] = "";
    uint8_t instruction;
    while (strcmp(input, "exit") != 0)
    {
        wrefresh(screen->input);
        mvwgetnstr(screen->input, 1, 1, input, 50);
        werase(screen->input);
        box(screen->input, 0, 0);

        if (strcmp(input, "step") == 0)
            instruction = step(&cpu);
        else if (strcmp(input, "s") == 0)
            instruction = step(&cpu);
        else if (strcmp(input, "run") == 0)
            instruction = run(&cpu);
    }

    destroy_screen(screen);

    return EXIT_SUCCESS;
}