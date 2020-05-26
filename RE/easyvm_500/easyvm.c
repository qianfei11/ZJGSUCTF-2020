// https://xz.aliyun.com/t/3851
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#define OPCODE_N 7-1

char *logo = "\033[34m\n ____ _____       _\n| __ )___ /  __ _| | ___\n|  _ \\ |_ \\ / _` | |/ _ \\\n| |_) |__) | (_| | |  __/\n|____/____/ \\__,_|_|\\___|\n\033[1m";

enum optypes
{
    OP_REG = 0xe0,
    OP_MEM = 0xe1,
};

// Define Opcodes
enum opcodes
{
    MOV = 0xf0,
    ADD = 0xf1,
    SUB = 0xf2,
    MUL = 0xf3,
    DIV = 0xf4,
    XOR = 0xf5,
    RET = 0xf6,
};

/*
MOV regs[0], vm_stack[0]
ADD regs[0], 20
MOV vm_stack[0x20+0], regs[0]
MOV regs[1], vm_stack[1]
SUB regs[1], 16
MOV vm_stack[0x20+1], regs[1]
MOV regs[2], vm_stack[2]
MUL regs[2], 1
MOV vm_stack[0x20+2], regs[2]
MOV regs[3], vm_stack[3]
DIV regs[3], 1
MOV vm_stack[0x20+3], regs[3]
MOV regs[4], vm_stack[4]
XOR regs[4], 73
MOV vm_stack[0x20+4], regs[4]
MOV regs[0], vm_stack[5]
ADD regs[0], 20
MOV vm_stack[0x20+5], regs[0]
MOV regs[1], vm_stack[6]
SUB regs[1], 16
MOV vm_stack[0x20+6], regs[1]
MOV regs[2], vm_stack[7]
MUL regs[2], 1
MOV vm_stack[0x20+7], regs[2]
MOV regs[3], vm_stack[8]
DIV regs[3], 1
MOV vm_stack[0x20+8], regs[3]
MOV regs[4], vm_stack[9]
XOR regs[4], 73
MOV vm_stack[0x20+9], regs[4]
MOV regs[0], vm_stack[10]
ADD regs[0], 20
MOV vm_stack[0x20+10], regs[0]
MOV regs[1], vm_stack[11]
SUB regs[1], 16
MOV vm_stack[0x20+11], regs[1]
MOV regs[2], vm_stack[12]
MUL regs[2], 1
MOV vm_stack[0x20+12], regs[2]
MOV regs[3], vm_stack[13]
DIV regs[3], 1
MOV vm_stack[0x20+13], regs[3]
MOV regs[4], vm_stack[14]
XOR regs[4], 73
MOV vm_stack[0x20+14], regs[4]
MOV regs[0], vm_stack[15]
ADD regs[0], 20
MOV vm_stack[0x20+15], regs[0]
*/
uint8_t vm_code[] = {
    MOV, OP_REG, 0, 0, ADD, 0, 20, MOV, OP_MEM, 0, 32,
    MOV, OP_REG, 1, 1, SUB, 1, 15, MOV, OP_MEM, 1, 33,
    MOV, OP_REG, 2, 2, MUL, 2, 1, MOV, OP_MEM, 2, 34,
    MOV, OP_REG, 3, 3, DIV, 3, 1, MOV, OP_MEM, 3, 35,
    MOV, OP_REG, 4, 4, XOR, 4, 73, MOV, OP_MEM, 4, 36,
    MOV, OP_REG, 0, 5, ADD, 0, 20, MOV, OP_MEM, 0, 37,
    MOV, OP_REG, 1, 6, SUB, 1, 15, MOV, OP_MEM, 1, 38,
    MOV, OP_REG, 2, 7, MUL, 2, 1, MOV, OP_MEM, 2, 39,
    MOV, OP_REG, 3, 8, DIV, 3, 1, MOV, OP_MEM, 3, 40,
    MOV, OP_REG, 4, 9, XOR, 4, 73, MOV, OP_MEM, 4, 41,
    MOV, OP_REG, 0, 10, ADD, 0, 20, MOV, OP_MEM, 0, 42,
    MOV, OP_REG, 1, 11, SUB, 1, 15, MOV, OP_MEM, 1, 43,
    MOV, OP_REG, 2, 12, MUL, 2, 1, MOV, OP_MEM, 2, 44,
    MOV, OP_REG, 3, 13, DIV, 3, 1, MOV, OP_MEM, 3, 45,
    MOV, OP_REG, 4, 14, XOR, 4, 73, MOV, OP_MEM, 4, 46,
    MOV, OP_REG, 0, 15, ADD, 0, 20, MOV, OP_MEM, 0, 47,
    RET
};
uint8_t ciphertext[] = {0x8e, 0x5b, 0x67, 0x73, 0x3c, 0x77, 0x65, 0x66, 0x7b, 0x3f, 0x61, 0x57, 0x55, 0x6e, 0x68, 0x91};
uint8_t *vm_stack;

// Opcode Structure
typedef struct
{
    uint8_t opcode;
    void (*handler)(void *);
} vm_opcode;

// CPU Structure
typedef struct
{
    int regs[5]; // Common Registers
    uint8_t *eip; // PC
    vm_opcode op_list[OPCODE_N]; // Opcode list
} vm_cpu;

void mov(vm_cpu *cpu);
void add(vm_cpu *cpu);
void sub(vm_cpu *cpu);
void mul(vm_cpu *cpu);
void div_(vm_cpu *cpu);
void xor_(vm_cpu *cpu);

void mov(vm_cpu *cpu)
{
    uint8_t choice = *(cpu->eip + 1);
    uint8_t regn = *(cpu->eip + 2);
    uint8_t offset = *(cpu->eip + 3);
    switch (choice)
    {
    case OP_REG:
        cpu->regs[regn] = *(vm_stack + offset);
        break;
    case OP_MEM:
        *(vm_stack + offset) = cpu->regs[regn];
        break;
    default:
        break;
    }
    cpu->eip += 4;
}

void add(vm_cpu *cpu)
{
    uint8_t regn = *(cpu->eip + 1);
    uint8_t data = *(cpu->eip + 2);
    cpu->regs[regn] += data;
    cpu->eip += 3;
}

void sub(vm_cpu *cpu)
{
    uint8_t regn = *(cpu->eip + 1);
    uint8_t data = *(cpu->eip + 2);
    cpu->regs[regn] -= data;
    cpu->eip += 3;
}

void mul(vm_cpu *cpu)
{
    uint8_t regn = *(cpu->eip + 1);
    uint8_t data = *(cpu->eip + 2);
    cpu->regs[regn] *= data;
    cpu->eip += 3;
}

void div_(vm_cpu *cpu)
{
    uint8_t regn = *(cpu->eip + 1);
    uint8_t data = *(cpu->eip + 2);
    cpu->regs[regn] /= data;
    cpu->eip += 3;
}

void xor_(vm_cpu *cpu)
{
    uint8_t regn = *(cpu->eip + 1);
    uint8_t data = *(cpu->eip + 2);
    cpu->regs[regn] ^= data;
    cpu->eip += 3;
}

void (*funcs[])(vm_cpu *) = {mov, add, sub, mul, div_, xor_};

void vm_init(vm_cpu *cpu)
{
    // Initialize registers
    for (int i = 0; i < 5; i++) {
        cpu->regs[i] = 0; 
    }
    cpu->eip = vm_code; // Point PC to vm_code
    for (int i = 0; i < OPCODE_N; i++)
    {
        cpu->op_list[i].opcode = 0xf0 + i;
        cpu->op_list[i].handler = (void (*)(void *))funcs[i];
    }
    vm_stack = malloc(0x9f8);
    memset(vm_stack, 0, sizeof(vm_stack));
}

void vm_dispacher(vm_cpu *cpu)
{
    for (int i = 0; i < OPCODE_N; i++)
    {
        if (*cpu->eip == cpu->op_list[i].opcode)
        {
            cpu->op_list[i].handler(cpu);
            break;
        }
    }
}

void vm_start(vm_cpu *cpu)
{
    cpu->eip = (uint8_t *)vm_code;
    while ((*cpu->eip) != RET)
    {
        vm_dispacher(cpu);
    }
}

void welcome()
{
    puts(logo);
}

void check()
{
    for (int i = 0; i < 16; i++) {
        if (*(vm_stack + 0x20 + i) != ciphertext[i]) {
            printf("Wrong.\n");
            exit(-1);
        }
    }
    printf("Right.\n");
    exit(0);
}

int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
    welcome();
    vm_cpu *cpu = NULL;
    vm_init(&cpu);
    printf("Shout out to zs0zrc!\n");
    printf("What's the flag? "); // zjgsuctf{vMfUn!}
    read(0, vm_stack, 0x10);
    vm_start(&cpu);
    check();
}
