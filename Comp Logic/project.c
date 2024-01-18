#include "spimcore.h"
#define ALU_ADD 0
#define ALU_SUB 1
#define ALU_SLT 2
#define ALU_SLTU 3
#define ALU_AND 4
#define ALU_XOR 5
#define ALU_SSL16 6
#define TYPE_R 7

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch (ALUControl) //ALU codes to actual operations
    {
    case ALU_ADD:
        *ALUresult = A + B;
        break;
    case ALU_SUB:
        *ALUresult = A - B;
        break;
    case ALU_SLT:
        *ALUresult = (int)A < (int)B;
        break;
    case ALU_SLTU:
        *ALUresult = A < B;
        break;
    case ALU_AND:
        *ALUresult = A & B;
        break;
    case ALU_XOR:
        *ALUresult = A ^ B;
        break;
    case ALU_SSL16:
        *ALUresult = B << 16;
        break;
    case TYPE_R: //not actually for type-Rs but uses same ALU code
        *ALUresult = ~A;
        break;
    default:
        *ALUresult = 0;
        break;
    }
    *Zero = (*ALUresult == 0) ? 1 : 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if(PC % 4 != 0 || PC < 0 || PC > 0xFFFF) { //check if word aligned and if outside mem bounds
        return 1;
    }
    *instruction = Mem[PC >> 2];
    return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = instruction >> 26;
    *r1 = instruction >> 21 & 0x1F;
    *r2 = instruction >> 16 & 0x1F;
    *r3 = instruction >> 11 & 0x1F;
    *funct = instruction & 0x3F;
    *offset = instruction & 0xFFFF;
    *jsec = instruction & 0x3FFFFFF;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    memset(controls, 0, sizeof(struct_controls));
    switch(op) { //op codes translation
        case 0x00: //R-type instructions
            controls->RegDst = 1;
            controls->RegWrite = 1;
            controls->ALUOp = TYPE_R;
            break;

        case 0x08: //addi
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = ALU_ADD;
            break;

        case 0x23: //lw
            controls->ALUSrc = 1;
            controls->MemRead = 1;
            controls->RegWrite = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = ALU_ADD;
            break;

        case 0x2B: //sw
            controls->ALUSrc = 1;
            controls->MemWrite = 1;
            controls->ALUOp = ALU_ADD;
            break;

        case 0x04: //beq
            controls->ALUSrc = 0;
            controls->Branch = 1;
            controls->ALUOp = ALU_SUB;
            break;

        case 0x0F: //lui
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = ALU_SSL16;
            break;

        case 0x0A: //slti
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = ALU_SLT;
            break;

        case 0x0B: //sltiu
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            controls->ALUOp = ALU_SLTU;
            break;

        case 0x02: //j
            controls->Jump = 1;
            break;

        default: //invalid Op
            return 1;
    }
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    if(offset & 0x8000) { //if 15th bit is 1 aka sign bit
        *extended_value = offset | 0xFFFF0000; //set upper 16 bits to 1 
    } else {
        *extended_value = offset;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    unsigned operandB = (ALUSrc == 0) ? data2 : extended_value; //source of second operand, 0 = data2, 1 = extended_value
    if(ALUOp == TYPE_R) {
        switch(funct) {
        case 0x20: //funct code translations
            ALUOp = ALU_ADD;
            break;
        case 0x22:
            ALUOp = ALU_SUB;
            break;
        case 0x24:
            ALUOp = ALU_AND;
            break;
        case 0x26:
            ALUOp = ALU_XOR;
            break;
        case 0x2A:
            ALUOp = ALU_SLT;
            break;
        case 0x2B:
            ALUOp = ALU_SLTU;
            break;
        default: //invalid funct
            return 1;
            break;
        }
    }

    ALU(data1, operandB, ALUOp, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    if(MemRead || MemWrite) { //check if ALUresult is an address and/or if mem op is needed
        if((ALUresult % 4) != 0 || ALUresult > 0xFFFF) { //check if word aligned and if outside mem bounds
            return 1;
        }
        if(MemWrite) { 
            Mem[ALUresult >> 2] = data2; 
        } else if(MemRead) {
            *memdata = Mem[ALUresult >> 2]; 
        }
    }
    return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite) {
        unsigned data = (MemtoReg) ? memdata : ALUresult;
        if(RegDst) {
            Reg[r3] = data;
        } else {
            Reg[r2] = data;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC += 4;
    if(Branch && Zero) { //since only need branch if equals
        *PC += extended_value << 2;
    }
    if(Jump) {
        *PC = (*PC & 0xF0000000) | (jsec << 2);
    }
}
