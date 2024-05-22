#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

typedef struct s_cpu
{
    uint8_t memory[256];
    uint8_t pc;
    uint8_t a;
    uint8_t b;
    uint8_t i;
    uint8_t flags;
} Cpu;

Cpu create_cpu();
void load_rom(Cpu *cpu, char *file_path);
void print_state(Cpu *cpu);
uint8_t step(Cpu *cpu);
void run(Cpu *cpu);

#endif