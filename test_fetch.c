#include <stdio.h>
#include "spimcore.h"


unsigned instruction;
unsigned global_ALUresult;
unsigned global_data2;
unsigned global_memdata;
struct_controls global_controls;



void test_instruction_fetch() {
    unsigned Mem[16384] = {0};
    // unsigned instruction;
    unsigned PC_valid = 0x4000;
    unsigned PC_invalid = 0x4002;

    Mem[PC_valid >> 2] = 0xDEADBEEF; // Illegal instruction
    Mem[PC_valid >> 2] = 0x21080001; // Legal i-type instruction
    // Mem[PC_valid >> 2] = 0x01095020; // Legal r-type instruction
    // Mem[PC_valid >> 2] = 0x8fab0000;
    // Mem[PC_valid >> 2] = 0x016a602a;
    Mem[PC_valid >> 2] = 0x8D0B0000;
    Mem[PC_valid >> 2] = 0x01095020; // add $10, $8, $9
    // printf("Test 1: Valid PC (0x%X)...\n", PC_valid);
    int result1 = instruction_fetch(PC_valid, Mem, &instruction);
    // printf("  Expected: 0, Actual: %d\n", result1);
    // printf("  Fetched instruction: 0x%08X\n", instruction);

    // printf("\nTest 2: Invalid PC (0x%08X)...\n", PC_invalid);
    int result2 = instruction_fetch(PC_invalid, Mem, &instruction);
    // printf("  Expected: 1, Actual: %d\n", result2);
}

void test_instruction_partition() {
    
    extern unsigned instruction;
    unsigned op, r1, r2, r3, funct, offset, jsec;

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    //printf("\nTest 3: instruction_partition on fetched instruction 0x%08X...\n", instruction);
    // printf("  op     = 0x%X\n", op);
    // printf("  r1     = %u\n", r1);
    // printf("  r2     = %u\n", r2);
    // printf("  r3     = %u\n", r3);
    // printf("  funct  = 0x%X\n", funct);
    // printf("  offset = 0x%X\n", offset);
    // printf("  jsec   = 0x%X\n", jsec);
}

void test_instruction_decode(){
    extern unsigned instruction;
    unsigned op, r1, r2, r3, funct, offset, jsec;
    struct_controls controls;

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    int result = instruction_decode(op, &controls);

    // printf("\nTest 4: instruction_decode for op = 0x%X\n", op);
    // printf("  Halt: %d\n", result);
    // printf("  RegDst   = %d\n", controls.RegDst);
    // printf("  Jump     = %d\n", controls.Jump);
    // printf("  Branch   = %d\n", controls.Branch);
    // printf("  MemRead  = %d\n", controls.MemRead);
    // printf("  MemtoReg = %d\n", controls.MemtoReg);
    // printf("  ALUOp    = %d\n", controls.ALUOp);
    // printf("  MemWrite = %d\n", controls.MemWrite);
    // printf("  ALUSrc   = %d\n", controls.ALUSrc);
    // printf("  RegWrite = %d\n", controls.RegWrite);
}

void test_read_register() {
    extern unsigned instruction; 
    unsigned Reg[32] = {0};
    unsigned op, r1, r2, r3, funct, offset, jsec;
    unsigned data1, data2;

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    Reg[r1] = 0x11111111;
    Reg[r2] = 0x22222222;

    read_register(r1, r2, Reg, &data1, &data2);

    // printf("\nTest 5: read_register using partitioned r1, r2 from instruction\n");
    // printf(" r1 = %u, Reg[r1] = 0x%08X\n", r1, data1);
    // printf(" r2 = %u, Reg[r2] = 0x%08X\n", r2, data2);
}

void test_sign_extend() {
    extern unsigned isntruction;
    unsigned op, r1, r2, r3, funct, offset, jsec;
    unsigned extended;

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    sign_extend(offset, &extended);

    // printf("\nTest 6: sign_extend\n");
    // printf("  Original offset = 0x%04X\n", offset);
    // printf("  Extended value = 0x%08X\n", extended);
}

void test_ALU_operations(){
    extern unsigned instruction;
    unsigned op, r1, r2, r3, funct, offset, jsec;
    unsigned ALUresult;
    char Zero;

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    unsigned Reg[32] = {0};
    // Reg[r1] = 0x00000008;
    // if (r2 != r1)
    //     Reg[r2] = 0x22222222;

    Reg[r1] = 1;
    if (r2 != r1) Reg[r2] = 3;

    unsigned data1, data2;
    read_register(r1, r2, Reg, &data1, &data2);

    unsigned extended;
    sign_extend(offset, &extended);

    // struct_controls controls;
    // instruction_decode(op, &controls);
    instruction_decode(op, &global_controls);

    // Reg[r1] = 8;
    ALU_operations(data1, data2, extended, funct, global_controls.ALUOp, global_controls.ALUSrc, &ALUresult, &Zero);
    global_ALUresult = ALUresult;
    global_data2 = data2;  // optional, useful if you want to test sw

    // printf("\nTest 7: ALU_operations\n");
    // printf("  ALU Input 1      = 0x%08X\n", Reg[r1]);
    // printf("  ALU Input 2      = 0x%08X (based on ALUSrc)\n", controls.ALUSrc ? extended : Reg[r2]);
    // printf("  ALUOp            = %d\n", controls.ALUOp);
    // printf("  Function code    = 0x%X\n", funct);
    // printf("  ALU result       = 0x%08X\n", ALUresult);
    // printf("  Zero flag        = %d\n", Zero);
}

void test_rw_memory() {
    printf("\nTest 8: rw_memory (lw)\n");
    unsigned Mem[16384] = {0};
    unsigned memdata;
    char MemWrite = global_controls.MemWrite;
    char MemRead = global_controls.MemRead;

    unsigned index = global_ALUresult / 4;
    if (index >= 16384) {
        printf("out of bounds");
        return;
    }
    // Mem[global_ALUresult / 4] = 0x8D0B0000;

    // int result = rw_memory(global_ALUresult, global_data2, MemWrite, MemRead, &memdata, Mem);
    // global_memdata = memdata;
    // printf("  Mem[%u] = 0x%08X\n", global_ALUresult / 4, Mem[global_ALUresult / 4]);
    // printf("  Read memdata = 0x%08X\n", memdata);
    // printf("  Result (halt flag) = %d\n", result);

    // printf("\nTest 9: rw_memory (sw)\n");
    // global_controls.MemWrite = 1;
    // global_controls.MemRead = 0;
    // global_data2 = 0xDEADBEEF;

    // result = rw_memory(global_ALUresult, global_data2, global_controls.MemWrite, global_controls.MemRead, &memdata, Mem);
    // printf("  Wrote 0x%08X to Mem[%u]\n", global_data2, global_ALUresult / 4);
    // printf("  Meme[%u] = 0x%08X\n", global_ALUresult / 4, Mem[global_ALUresult / 4]);
    // printf("  Result (halt flag) = %d\n", result);
}


void test_write_register() {
    printf("\nTest 10: write_register\n");

    unsigned Reg[32] = {0};
    unsigned op, r1, r2, r3, funct, offset, jsec;

    instruction_partition(instruction, &op, &r1, &r2, &r3, &funct, &offset, &jsec);

    global_controls.RegWrite = 1;
    global_controls.RegDst = 1;
    global_controls.MemtoReg = 0;

    write_register(r2, r3, global_memdata, global_ALUresult, global_controls.RegWrite, global_controls.RegDst, global_controls.MemtoReg, Reg);

    unsigned dest = (global_controls.RegDst == 1) ? r3 : r2;
    printf("  Expected write to reg[%u]\n", dest);
    printf("  Reg[%u] = 0x%08X\n", dest, Reg[dest]);
}

void test_PC_update() {
    printf("\nTest 11: PC_update\n");

    unsigned PC;

    // --- Case 1: Normal PC increment ---
    PC = 0x00400000;
    PC_update(0, 0, 0, 0, 0, &PC);
    printf("  Case 1 - Normal increment: PC = 0x%08X (expected 0x00400004)\n", PC);

    // --- Case 2: Branch taken ---
    PC = 0x00400000;
    PC_update(0, 0x0002, 1, 0, 1, &PC); // extended_value = 2, shifted left by 2 → 8
    printf("  Case 2 - Taken branch: PC = 0x%08X (expected 0x0040000C)\n", PC);

    // --- Case 3: Jump ---
    PC = 0x00400000;
    PC_update(0x0003FFFF, 0, 0, 1, 0, &PC); // jsec << 2 = 0x00FFFFFC
    printf("  Case 3 - Jump: PC = 0x%08X (expected 0x00FFFFFC)\n", PC);
}

void test_ALU() {
    printf("\nTest 12: ALU (direct)\n");

    unsigned A = 0x00000005;
    unsigned B = 0x00000003;
    unsigned result;
    char zero;

    for (char control = 0; control <= 7; control++) {
        ALU(A, B, control, &result, &zero);
        printf("  ALUControl = %d → ", control);
        switch (control) {
            case 0:  printf("ADD       "); break;
            case 1:  printf("SUB       "); break;
            case 2:  printf("SLT       "); break;
            case 3:  printf("SLTU      "); break;
            case 4:  printf("AND       "); break;
            case 5:  printf("OR        "); break;
            case 6:  printf("SLL 16    "); break;
            case 7:  printf("NOT       "); break;
        }
        printf("→ Result: 0x%08X, Zero: %d\n", result, zero);
    }
}

int main() {
    test_instruction_fetch();
    test_instruction_partition();
    test_instruction_decode();
    test_read_register();
    test_sign_extend();
    test_ALU_operations();
    test_rw_memory();
    test_write_register();
    test_PC_update();
    test_ALU();
    return 0;
}