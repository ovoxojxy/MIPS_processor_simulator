#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    unsigned int Z = 0;
    //a and b for signed comparison
    int a = A;
    int b = B;
    switch (ALUControl)
    {
        case 0:     //Z = A + B
            Z = A + B;
            break;
        case 1:     //Z = A - B
            Z = A - B;
            break;
        case 2:     //Z = 1 if A < B (signed)
            Z = (a < b);
            break;
        case 3:     //Z = 1 if A < B (unsigned)
            Z = (A < B);
            break;
        case 4:     //A AND B
            Z = (A & B);    
            break;
        case 5:     //A OR B
            Z = (A | B);
            break;
        case 6:     //Z = Shift B left by 16 bits
            Z = (B << 16);
            break;
        case 7:     //Z = NOT A
            Z = (~A);
            break;
        default:
            return;
            break;
    }
    
    //assign ALUresult and Zero
    *ALUresult = Z;

    if(Z == 0)
        *Zero = 1;
    else 
        *Zero = 0;    
    
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if(PC % 4 == 0) //check if word-aligned
    {
        //set instruction
        *instruction = Mem[PC >> 2];

        return 0;  
    }   
    
    //return halt if not word-aligned
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
    unsigned r2_mask = generate_mask(20, 5);
    unsigned r3_mask = generate_mask(15, 5);
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

    if (op == 2){
        // Jump signal control
        controls->RegDst = 2;
        controls->Jump = 1;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 2;
        controls->RegWrite = 0;

    } else if (op == 0){
        // R-type signal controls
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 7;  
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;

    } else {
        switch(op) {
            case 43:
                // store word
                controls->RegDst = 2;
                controls->Jump = 0;
                controls->Branch = 0;
                controls->MemRead = 0;
                controls->MemtoReg = 2;
                controls->ALUOp = 0;  
                controls->MemWrite = 1;
                controls->ALUSrc = 1;
                controls->RegWrite = 0;

                break;
            case 35:
                // load word 
                controls->RegDst = 0;
                controls->Jump = 0;
                controls->Branch = 0;
                controls->MemRead = 1;
                controls->MemtoReg = 1;
                controls->ALUOp = 0;  
                controls->MemWrite = 0;
                controls->ALUSrc = 1;
                controls->RegWrite = 1;

                break;
            case 15:
                // load upper immediate
                controls->RegDst = 0;
                controls->Jump = 0;
                controls->Branch = 0;
                controls->MemRead = 0;
                controls->MemtoReg = 0;
                controls->ALUOp = 2;  
                controls->MemWrite = 0;
                controls->ALUSrc = 1;
                controls->RegWrite = 1;

                break;
            case 8:
                // add immediate
                controls->RegDst = 0;
                controls->Jump = 0;
                controls->Branch = 0;
                controls->MemRead = 0;
                controls->MemtoReg = 0;
                controls->ALUOp = 0;  
                controls->MemWrite = 0;
                controls->ALUSrc = 1;
                controls->RegWrite = 1;

                break;
            case 10:
                // set less than immediate
                controls->RegDst = 0;
                controls->Jump = 0;
                controls->Branch = 0;
                controls->MemRead = 0;
                controls->MemtoReg = 0;
                controls->ALUOp = 3;  
                controls->MemWrite = 0;
                controls->ALUSrc = 1;
                controls->RegWrite = 1;

                break;
            case 4:
                // branch on equal
                controls->RegDst = 2;
                controls->Jump = 0;
                controls->Branch = 1;
                controls->MemRead = 0;
                controls->MemtoReg = 2;
                controls->ALUOp = 1;  
                controls->MemWrite = 0;
                controls->ALUSrc = 0;
                controls->RegWrite = 0;

                break;
            case 11:
                // set less than immediate
                controls->RegDst = 0;
                controls->Jump = 0;
                controls->Branch = 0;
                controls->MemRead = 0;
                controls->MemtoReg = 0;
                controls->ALUOp = 3;  
                controls->MemWrite = 0;
                controls->ALUSrc = 1;
                controls->RegWrite = 1;
                break;
            default:
                return 1;
        }
    }
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Read registers addressed by r1 and r2 from Reg
    // Write read values to data1 and data2
    
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // Assign sign extended value of offset to extended_value
    
    // Check if the left most bit is negative or positive
    if(offset & (1 << 15)) {
        *extended_value = offset | 0xFFFF0000; // Sign extend with 1s for negative
    } else  {
        *extended_value = offset & 0x0000FFFF; // If it's positive extend with 0s
    }
    
    
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value,
                   unsigned funct, char ALUOp, char ALUSrc,
                   unsigned *ALUresult, char *Zero)
{

    char ALUControl;

    if (ALUOp == 0)        // LW or SW
        ALUControl = 0;
    else if (ALUOp == 1)   // BEQ
        ALUControl = 1;
    else if (ALUOp == 7)   // The R-type instruction
    {
        switch (funct)
        {
            case 32: ALUControl = 0; break;  // ADD
            case 34: ALUControl = 1; break;  // SUB
            case 42: ALUControl = 2; break;  // SLT
            case 43: ALUControl = 3; break;  // SLTU
            case 36: ALUControl = 4; break;  // AND
            case 37: ALUControl = 5; break;  // OR
            case 0:  ALUControl = 6; break;  // SLL (shift left)
            case 39: ALUControl = 7; break;  // NOR (use NOT here)
            default: return 1; // Invalid funct
        }
    }
    else if(ALUOp == 2) //for load upper immediate
    {
        ALUControl = 6;
    }
    else
        return 1; // Invalid ALUOp


    unsigned operand2 = ALUSrc ? extended_value : data2;
    ALU(data1, operand2, ALUControl, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

    if(!MemWrite && !MemRead){  //checks if neither MemWrite nor MemRead is occurring
        return  0;
    }
    if(ALUresult % 4 != 0) { //check for word alignment on address
        return 1;   //returns 1 for halt if not word aligned
    }    
    unsigned index = ALUresult / 4; //convert ALUresult from byte address to word index

    if(MemRead == 1){
        *memdata = Mem[index];  //reads the content in the memory location to memdata
    }
    if(MemWrite == 1){            
        Mem[index] = data2;   //writes value of data2 to memory location
    }
    return 0;   //no halt condition occurs
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite && MemtoReg == 1) { //write data from memory
        unsigned dest;
        if(RegDst == 1){  //determines the destination register
            dest = r3;
        } else {
            dest = r2;
        }
        if(dest != 0) {   //write to registers other than $zero
            Reg[dest] = memdata;
        }
    }
    if(RegWrite == 1 && MemtoReg == 0) { //write data from ALUresult
        unsigned dest;
        if(RegDst == 1) {  //determines the destination register
            dest = r3;
        } else {
            dest = r2;
        }
        if(dest != 0) {  //write to registers other than $zero
            Reg[dest] = ALUresult;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    *PC += 4;  // move it to the next instruction

    if (Branch && Zero)
    {
        *PC += (extended_value << 2);  // branch the target
    }

    if (Jump)
    {
        *PC = (*PC & 0xF0000000) | (jsec << 2);  // jump the target address
    }
}