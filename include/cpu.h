#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <ncurses.h>
#include "screen.h"

typedef struct s_cpu
{
    Screen *screen;
    uint8_t memory[256];
    uint8_t pc;
    uint8_t a;
    uint8_t b;
    uint8_t i;
    uint8_t flags;
    uint8_t running;
} Cpu;

Cpu create_cpu(Screen *screen);
void draw_screen(Cpu *cpu);
void load_rom(Cpu *cpu, char *file_path);
uint8_t step(Cpu *cpu);
uint8_t run(Cpu *cpu);

#endif