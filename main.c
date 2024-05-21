#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"


void main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Missing ROM name.\n");
        exit(EXIT_FAILURE);
    }

    Cpu cpu = create_cpu();
    load_rom(&cpu, argv[1]);
    run(&cpu);
}