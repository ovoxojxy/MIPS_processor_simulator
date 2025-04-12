#include <stdio.h>
#include "spimcore.h"

void test_ALU() {
    unsigned result;
    char zero;

    printf("Testing ALU...\n");

    ALU(5, 3, 0b000, &result, &zero); // ADD
    printf("ADD: %u, Zero: %d\n", result, zero);

    ALU(5, 5, 0b001, &result, &zero); // SUB
    printf("SUB: %u, Zero: %d\n", result, zero);

    ALU(1, 2, 0b010, &result, &zero); // SLT
    printf("SLT: %u\n", result);

    ALU(0xFFFFFFFF, 0, 0b011, &result, &zero); // SLTU
    printf("SLTU: %u\n", result);

    ALU(0xF0F0F0F0, 0x0F0F0F0F, 0b100, &result, &zero); // AND
    printf("AND: %08X\n", result);

    ALU(0xF0F0F0F0, 0x0F0F0F0F, 0b101, &result, &zero); // OR
    printf("OR: %08X\n", result);

    ALU(0, 1, 0b110, &result, &zero); // LUI
    printf("LUI: %08X\n", result);

    ALU(0xFFFFFFFF, 0, 0b111, &result, &zero); // NOT
    printf("NOT: %08X\n", result);
}

void test_sign_extend() {
    unsigned extended;
    printf("\nTesting sign_extend...\n");

    sign_extend(0x7FFF, &extended);
    printf("0x7FFF => 0x%08X\n", extended);

    sign_extend(0x8000, &extended);
    printf("0x8000 => 0x%08X\n", extended);

    sign_extend(0xFFFF, &extended);
    printf("0xFFFF => 0x%08X\n", extended);
}

void test_instruction_partition() {
    unsigned op, r1, r2, r3, funct, offset, jsec;
    printf("\nTesting instruction_partition...\n");

    instruction_partition(0x012A4020, &op, &r1, &r2, &r3, &funct, &offset, &jsec);
    printf("op=%u, r1=%u, r2=%u, r3=%u, funct=%u, offset=0x%04X, jsec=0x%06X\n", op, r1, r2, r3, funct, offset, jsec);
}

void test_instruction_fetch() {
    printf("\nTesting instruction_fetch...\n");

    unsigned Mem[1024] = {0};
    Mem[1] = 0x012A4020;
    unsigned instr;
    int halt = instruction_fetch(4, Mem, &instr);
    printf("Fetched: 0x%08X, Halt: %d\n", instr, halt);
}

void test_rw_memory() {
    printf("\nTesting rw_memory...\n");

    unsigned Mem[1024] = {0};
    unsigned memdata = 0;
    int halt;

    halt = rw_memory(8, 0xDEADBEEF, 1, 0, &memdata, Mem);
    printf("Write Mem[2] = 0x%08X, Halt: %d\n", Mem[2], halt);

    Mem[2] = 0xCAFEBABE;
    halt = rw_memory(8, 0, 0, 1, &memdata, Mem);
    printf("Read Mem[2] => 0x%08X, Halt: %d\n", memdata, halt);
}

void test_read_register() {
    printf("\nTesting read_register...\n");

    unsigned Reg[35] = {0};
    Reg[5] = 100;
    Reg[10] = 200;
    unsigned d1, d2;
    read_register(5, 10, Reg, &d1, &d2);
    printf("data1=%u, data2=%u\n", d1, d2);
}

void test_write_register() {
    printf("\nTesting write_register...\n");

    unsigned Reg[35] = {0};
    write_register(2, 3, 0x12345678, 0xDEADBEEF, 1, 1, 0, Reg); // Write ALU result to r3
    printf("Reg[3] = 0x%08X\n", Reg[3]);

    write_register(2, 3, 0xBEEFCAFE, 0x0, 1, 0, 1, Reg); // Write memdata to r2
    printf("Reg[2] = 0x%08X\n", Reg[2]);
}

void test_PC_update() {
    printf("\nTesting PC_update...\n");

    unsigned PC = 0x4000;
    PC_update(0x004000, 4, 1, 0, 1, &PC); // branch
    printf("PC after branch: 0x%08X\n", PC);

    PC = 0x4000;
    PC_update(0x004000, 0, 0, 1, 0, &PC); // jump
    printf("PC after jump: 0x%08X\n", PC);
}

int main() {
    test_ALU();
    test_sign_extend();
    test_instruction_partition();
    test_instruction_fetch();
    test_rw_memory();
    test_read_register();
    test_write_register();
    test_PC_update();

    return 0;
}