#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <ncurses.h>

typedef struct s_screen
{
    WINDOW *main_window;
    WINDOW *main_memory;
    WINDOW *instructions;
    WINDOW *registers;
    WINDOW *input;
    int x, y;
} Screen;

typedef struct s_cpu
{
    Screen *screen;
    uint8_t memory[256];
    uint8_t pc;
    uint8_t a;
    uint8_t b;
    uint8_t i;
    uint8_t flags;
} Cpu;

Cpu create_cpu(Screen *screen);
void load_rom(Cpu *cpu, char *file_path);
uint8_t step(Cpu *cpu);
void run(Cpu *cpu);

Screen create_screen();
void init_screen(Screen *screen);
void draw_screen(Cpu *cpu);
void draw_memory_address(Cpu *cpu, uint8_t address, uint8_t colored);

#endif