#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cpu.h"

Cpu create_cpu()
{
    Cpu cpu;
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
        *value = cpu->b;
        address = &cpu->a;
        break;
    case 0x01:
        break;
    case 0x02:
        break;
    case 0x03:
        break;
    case 0x04:
        break;
    case 0x05:
        break;
    case 0x06:
        *value = cpu->memory[cpu->pc++];
        address = &cpu->a;
        break;
    case 0x07:
        *value = cpu->memory[cpu->pc++];
        address = &cpu->b;
        break;
    case 0x08:
        break;
    case 0x09:
        break;
    case 0x0A:
        break;
    case 0x0B:
        break;
    case 0x0C:
        break;
    case 0x0D:
        break;
    }

    return address;
}

void add(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    *target += value;
}

void sub(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    *target -= value;
}

void mov(Cpu *cpu, uint8_t addressing)
{
    uint8_t value;
    uint8_t *target = handle_address(cpu, addressing, &value);
    *target = value;
}

void cmp(Cpu *cpu, uint8_t addressing)
{
}

void and (Cpu * cpu, uint8_t addressing)
{
}

void or (Cpu * cpu, uint8_t addressing)
{
}

void run(Cpu *cpu)
{
    int instruction;
    do
    {
        instruction = cpu->memory[cpu->pc++];
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
            printf("inc\n");
            break;
        case 0x04:
            printf("dec\n");
            break;
        case 0x05:
            and(cpu, addressing);
            break;
        case 0x06:
            or (cpu, addressing);
            break;
        case 0x07:
            printf("not\n");
            break;
        case 0x08:
            printf("shr\n");
            break;
        case 0x09:
            printf("shl\n");
            break;
        case 0x0A:
            printf("jump\n");
            break;
        case 0x0B:
            mov(cpu, addressing);
            break;
        case 0x0F:
            printf("Exiting...\n");
            break;
        }
    } while (instruction != 0xFF);
}