#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cpu.h"

void draw_memory_address(Cpu *cpu, uint8_t address, uint8_t colored)
{
    if (colored)
        wattron(cpu->screen->memory, COLOR_PAIR(1));
    if (address % 0x10 == 0)
        wprintw(cpu->screen->memory, "\n  %.2x: ", address);
    wprintw(cpu->screen->memory, "%.2x", cpu->memory[address]);
    if ((address - 1) % 0x02 == 0)
        wprintw(cpu->screen->memory, " ");
    wattroff(cpu->screen->memory, COLOR_PAIR(1));
}

void draw_memory(Cpu *cpu)
{
    for (int i = 0x00; i < 0x100; i++)
        draw_memory_address(cpu, i, 0);
    box(cpu->screen->memory, 0, 0);
    mvwprintw(cpu->screen->memory, 0, 1, "MAIN MEMORY");
    wrefresh(cpu->screen->memory);
}

void draw_instructions(Cpu *cpu)
{
    box(cpu->screen->instructions, 0, 0);
    mvwprintw(cpu->screen->instructions, 0, 1, "INSTRUCTIONS");
    wrefresh(cpu->screen->instructions);
}

void draw_registers(Cpu *cpu)
{
    WINDOW *s = cpu->screen->registers;
    wmove(s, 1, 1);
    wprintw(s, "PC=%.2x  A=%.2x  B=%.2x  I=%.2x  FLAGS=%.2x", cpu->pc, cpu->a, cpu->b, cpu->i, cpu->flags);
    box(s, 0, 0);
    mvwprintw(s, 0, 1, "REGISTERS");
    wrefresh(s);
}

void draw_screen(Cpu *cpu)
{
    draw_memory(cpu);
    draw_instructions(cpu);
    draw_registers(cpu);
    wrefresh(cpu->screen->input);
    box(cpu->screen->input, 0, 0);
}

Cpu create_cpu(Screen *screen)
{
    Cpu cpu;
    cpu.running = 1;
    cpu.screen = screen;
    memset(cpu.memory, 0, 256);
    cpu.pc = 0;
    cpu.a = 0;
    cpu.b = 0;
    cpu.i = 0;
    cpu.flags = 0;

    return cpu;
}

void load_rom(Cpu *cpu, char *file_path)
{
    FILE *f = fopen(file_path, "rb");
    if (f == NULL)
    {
        printf("Error opening ROM file.\n");
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size >= 80)
    {
        printf("ROM exceeded limit amount (80B).\n");
        exit(EXIT_FAILURE);
    }

    fread(cpu->memory, size, 1, f);
}

uint8_t *handle_address(Cpu *cpu, uint8_t addressing, uint8_t *value)
{
    uint8_t *address = NULL;
    switch (addressing)
    {
    case 0x00:
        address = &cpu->a;
        *value = cpu->b;
        break;
    case 0x01:
        address = &cpu->b;
        *value = cpu->a;
        break;
    case 0x02:
        address = &cpu->a;
        *value = cpu->i;
        break;
    case 0x03:
        address = &cpu->i;
        *value = cpu->a;
        break;
    case 0x04:
        address = &cpu->a;
        *value = cpu->memory[cpu->i];
        break;
    case 0x05:
        address = &cpu->memory[cpu->i];
        *value = cpu->a;
        break;
    case 0x06:
        address = &cpu->a;
        *value = cpu->memory[cpu->pc++];
        break;
    case 0x07:
        address = &cpu->b;
        *value = cpu->memory[cpu->pc++];
        break;
    case 0x08:
        address = &cpu->i;
        *value = cpu->memory[cpu->pc++];
        break;
    case 0x09:
        address = &cpu->memory[cpu->i];
        *value = cpu->memory[cpu->pc++];
        break;
    case 0x0A:
        address = &cpu->a;
        *value = cpu->memory[cpu->memory[cpu->pc++]];
        break;
    case 0x0B:
        address = &cpu->b;
        *value = cpu->memory[cpu->memory[cpu->pc++]];
        break;
    case 0x0C:
        address = &cpu->memory[cpu->memory[cpu->pc++]];
        *value = cpu->a;
        break;
    case 0x0D:
        address = &cpu->memory[cpu->memory[cpu->pc++]];
        *value = cpu->b;
        break;
    }

    return address;
}

uint8_t *handle_single_address(Cpu *cpu, uint8_t addressing)
{
    uint8_t *address = NULL;
    switch (addressing)
    {
    case 0x00:
        address = &cpu->a;
        break;
    case 0x01:
        address = &cpu->b;
        break;
    case 0x02:
        address = &cpu->i;
        break;
    case 0x04:
        address = &cpu->memory[cpu->i];
        break;
    }

    return address;
}

uint8_t check_parity(uint8_t byte)
{
    return 0; // WIP
}

uint8_t get_flag(uint8_t flags[5])
{
    uint8_t flag = 0x0;

    for (int i = 0; i < 5; i++)
        flag |= flags[4 - i] << i;

    return flag;
}

void add(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    uint16_t result = *target + value;
    *target = result & 0xFF;

    uint8_t flags[5] = {result > 0xFF, result > 0xFF, 0, check_parity(result), 0};
    cpu->flags = get_flag(flags);
}

void sub(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    uint8_t negative = *target < value;
    uint8_t result = *target - value;
    *target = 0x100 - result;

    uint8_t flags[5] = {0, negative, result == 0, check_parity(result), negative};
    cpu->flags = get_flag(flags);
}

void mov(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    *target = value;
}

void cmp(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    int16_t result = *target - value;

    uint8_t flags[5] = {0, 0, result == 0, check_parity(result), result < 0};
    cpu->flags = get_flag(flags);
}

void inc(Cpu *cpu, uint8_t addressing)
{
    uint8_t *target = handle_single_address(cpu, addressing);
    uint16_t result = *target + 1;
    *target = result & 0xFF;

    uint8_t flags[5] = {result > 0xFF, result > 0xFF, 0, check_parity(result), 0};
    cpu->flags = get_flag(flags);
}

void dec(Cpu *cpu, uint8_t addressing)
{
    uint8_t *target = handle_single_address(cpu, addressing);
    int16_t result = *target - 1;
    *target = abs(result) & 0xFF;

    uint8_t flags[5] = {0, 0, result == 0, check_parity(result), result < 0};
    cpu->flags = get_flag(flags);
}

void and (Cpu * cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    uint8_t result = *target & value;
    *target = result;

    uint8_t flags[5] = {0, 0, result == 0, check_parity(result), 0};
    cpu->flags = (cpu->flags & 0x18) | get_flag(flags); // mask only the zf, pf, sf
}

void or (Cpu * cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    uint8_t result = *target | value;
    *target = result;

    uint8_t flags[5] = {0, 0, result == 0, check_parity(result), 0};
    cpu->flags = (cpu->flags & 0x18) | get_flag(flags); // mask only the zf, pf, sf
}

void not(Cpu * cpu, uint8_t addressing)
{
    uint8_t *target = handle_single_address(cpu, addressing);
    uint8_t result = ~*target;
    *target = result;

    uint8_t flags[5] = {0, 0, result == 0, check_parity(result), 0};
    cpu->flags = (cpu->flags & 0x18) | get_flag(flags); // mask only the zf, pf, sf
}

void shr(Cpu *cpu, uint8_t addressing)
{
    uint8_t *target = handle_single_address(cpu, addressing);
    uint8_t carry = *target & 0x01;
    uint8_t result = *target >> 1;
    *target = result;

    uint8_t flags[5] = {0, carry, result == 0, check_parity(result), 0};
    cpu->flags = get_flag(flags);
}

void shl(Cpu *cpu, uint8_t addressing)
{
    uint8_t *target = handle_single_address(cpu, addressing);
    uint8_t carry = *target & 0x01;
    uint8_t result = *target << 1;
    *target = result;

    uint8_t flags[5] = {carry, carry, result == 0, check_parity(result), 0};
    cpu->flags = get_flag(flags);
}

void jump(Cpu *cpu, uint8_t addressing)
{
    switch (addressing)
    {
    case 0x00:
        cpu->pc = cpu->memory[cpu->pc];
        return;
    case 0x01:
        if (cpu->flags & 0x04)
        {
            cpu->pc = cpu->memory[cpu->pc];
            return;
        }
        break;
    case 0x02:
        if (cpu->flags & 0x01)
        {
            cpu->pc = cpu->memory[cpu->pc];
            return;
        }
        break;
    case 0x03:
        if (cpu->flags & 0x08)
        {
            cpu->pc = cpu->memory[cpu->pc];
            return;
        }
        break;
    case 0x04:
        if (cpu->flags & 0x10)
        {
            cpu->pc = cpu->memory[cpu->pc];
            return;
        }
        break;
    case 0x05:
        if (cpu->flags & 0x02)
        {
            cpu->pc = cpu->memory[cpu->pc];
            return;
        }
        break;
    }

    cpu->pc++;
}

uint8_t step(Cpu *cpu)
{
    uint8_t instruction = cpu->memory[cpu->pc++];
    uint8_t op = (instruction & 0xF0) >> 4;
    uint8_t addressing = instruction & 0x0F;

    switch (op)
    {
    case 0x00:
        add(cpu, addressing);
        break;
    case 0x01:
        sub(cpu, addressing);
        break;
    case 0x02:
        cmp(cpu, addressing);
        break;
    case 0x03:
        inc(cpu, addressing);
        break;
    case 0x04:
        dec(cpu, addressing);
        break;
    case 0x05:
        and(cpu, addressing);
        break;
    case 0x06:
        or (cpu, addressing);
        break;
    case 0x07:
        not(cpu, addressing);
        break;
    case 0x08:
        shr(cpu, addressing);
        break;
    case 0x09:
        shl(cpu, addressing);
        break;
    case 0x0A:
        jump(cpu, addressing);
        break;
    case 0x0B:
        mov(cpu, addressing);
        break;
    }

    draw_screen(cpu);

    return instruction;
}

uint8_t run(Cpu *cpu)
{
    uint8_t instruction;
    do
    {
        instruction = step(cpu);
    } while (instruction != 0xFF);

    return instruction;
}