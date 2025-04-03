#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

    
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

    if(PC % 4 == 0) //check if word-aligned
    {
        *instruction = Mem[PC >> 2];

        return 0;  
    }
    
    printf("halt = 1\n"); //test print
    return 1;
}


/* instruction partition */
/* 10 Points */

// Bit mask generator

unsigned generate_mask(int start, int length) {

    return ((1 << length) - 1) << (start - length + 1);
}

void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    unsigned op_mask = generate_mask(32, 7);
    unsigned r1_mask = generate_mask(26, 6);
    unsigned r2_mask = generate_mask(21, 6);
    unsigned r3_mask = generate_mask(16, 6);
    unsigned funct_mask = generate_mask(6, 7);
    unsigned offset_mask = generate_mask(16, 17);
    unsigned jsec_mask = generate_mask(26, 27);

    *op = (instruction & op_mask) >> 26;
    *r1 = (instruction & r1_mask) >> 21;
    *r2 = (instruction & r2_mask) >> 16;
    *r3 = (instruction & r3_mask) >> 11;
    *funct = (instruction & funct_mask);
    *offset = (instruction & offset_mask);
    *jsec = (instruction & jsec_mask);
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Read registers addressed by r1 and r2 from Reg
    // Write read values to data1 and data2
    
    *data1 = Reg[r1];
    printf("data 1 = %u\n", data1); // Debug to check data 1
    *data2 = Reg[r2];
    printf("data 2 = %u\n", data2); // Debug to check data 2
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // Assign sign extended value of offset to extended_value
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    
}

