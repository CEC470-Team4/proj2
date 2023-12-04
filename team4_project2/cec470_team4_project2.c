// Authors: Milan Haruyama, Maegan Lucas, Tabitha O'Malley (Team 4)
// Professor: Dr. Laxima Niure Kandel
// Course: CEC 470
// Overview: Two-stage instruction decoder written in C.
//           Fetching written by Maegan Lucas.
//           Execution written by Milan Haruyama and Tabitha O'Malley.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Special
#define HALT_OPCODE 0x19 // 0b0001_1001
#define NOP_OPCODE 0x18 // 0b0001_1000

// Math
#define MATH_MASK 0x80 // 0b1000_0000
#define MATH_OPCODE 0x01 // 0b0001
#define MATH_FUNC_MASK 0x70 // 0b0111_0000
#define MATH_DST_MASK 0x0C // 0b01100
#define MATH_SRC_MASK 0x03 // 0b0011

// Memory
#define MEM_MASK 0xF0 // 0b1111_0000
#define MEM_OPCODE 0x0000 // 0b0000
#define MEM_FUNC_MASK 0x08 // 0b1000_0000
#define MEM_REG_MASK 0x04 // 0b0100
#define MEM_METH_MASK 0x03 // 0b0011

// Branch
#define BRANCH_MASK 0xF8 // 0b1111_1000
#define BRANCH_OPCODE 0x02 // 0b0_0010
#define BRANCH_TYPE_MASK 0x07 // 0b0111

FILE *fileptr;

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;

void readMemory(void);

void fetchNextInstruction(void);
void executeInstruction(void);

unsigned int address(void);

void mathOp(void);
unsigned int * mathOpDst(void);
unsigned int mathOpSrc(void);

void memOp(void);
unsigned int memOpMeth(void);

void branch(void);

int main(int argc, char * argv[])
{
    // read memory from file and save to memory array
    readMemory();

    // execution loop:
    // continue fetching and executing
    // until PC points to a HALT instrction

    while (memory[PC] != HALT_OPCODE)
    {
        fetchNextInstruction();
        executeInstruction();
    }
    printf("end loop\n");

    return 0;
}

void readMemory(void) // Maegan
{
    fileptr = fopen("mem_in.txt", "r");
    int i = 0;
    char row[CHAR_MAX];
    char *newRow;
    char *byte;

    while(1) {
        fgets(row, CHAR_MAX, fileptr);

        newRow = strtok(row, "\n");
        byte = strtok(newRow, " ");

        while(byte != NULL) {
            memory[i++] = (unsigned int) strtol(byte, NULL, 16);
            byte = strtok(NULL, " ");
        }
        if(feof(fileptr)) {
            break;
        }
    }
    fclose(fileptr);
}

void fetchNextInstruction(void) // Maegan & Milan
{
    int bytes = 0;
    IR = memory[PC];

    // Math Operation
    if ((IR & MATH_MASK) >> 7 == MATH_OPCODE) {
        
        // FUNC is INC, DEC, NOT
        if ( (IR & MATH_FUNC_MASK) >> 4 > 0x04)
        {
            //DST is a memory address
            if ((IR & MATH_DST_MASK) >> 2 == 0x03)
                bytes = 2;
            else
                bytes = 1;
        }

        //SRC is *MAR or ACC AND DST is not a memory address
        else if ( ((IR & MATH_SRC_MASK) >> 1 == 0x00) && ((IR & MATH_DST_MASK) >> 2 != 0x03))
            bytes = 1;

        //DST is *MAR or ACC AND SRC is constant
        else if ( ((IR & MATH_DST_MASK) >> 3 == 0x00) && ( (IR & MATH_SRC_MASK) == 0x02))
            bytes = 2;

        //destination OR source are an address
        else
            bytes = 3;
    }

    // Memory Operation
    else if ((IR & MEM_MASK) >> 4 == MEM_OPCODE) 
    {
        //METH is *MAR
        if ((IR & MEM_METH_MASK) == 0x02) 
            bytes = 1;

        //REG is ACC AND METH is constant
        else if (((IR & MEM_REG_MASK) >> 2 == 0x00) && ((IR & MEM_METH_MASK) == 0x01))
            bytes = 2;
        else
            bytes = 3;

    } 

    // Branch Operation
    else if ((IR & BRANCH_MASK) >> 3 == BRANCH_OPCODE)
        bytes = 3;

    // Special/Illegal Operations
    else
        bytes = 1;

    PC += bytes;
}

void executeInstruction(void) // Milan & Tabitha
{
    // check for regular opcodes first //

    // Mathematical operations
    if ((IR & MATH_MASK) >> 7 == MATH_OPCODE)
        mathOp();

    // Memory operations
    else if ((IR & MEM_MASK) >> 4 == MEM_OPCODE)
        memOp();

    // Branches/Jumps
    else if ((IR & BRANCH_MASK) >> 3 == BRANCH_OPCODE)
        branch();

    // check for HALT, NOP, or illegal opcodes last //
    else 
    switch(IR)
    {
        // HALT
        case HALT_OPCODE:
            // halts program
            break;

        // NOP
        case NOP_OPCODE:
            // literally does nothing lol
            break;

        // illegal opcode (default)
        default:
            // prints an error message
            printf("Error: Illegal opcode!\n"); 
            break;
    }

}

unsigned int address() // Milan
{
    // retrieves two byte address proceeding the opcode
    // also converts address from big-endian to little-endian
    return (memory[PC - 2] << 8) + memory[PC - 1];
}

void mathOp() // Milan & Tabitha
{
    unsigned int * dst = mathOpDst();
    unsigned int src = mathOpSrc();

    switch( (IR & MATH_FUNC_MASK) >> 4)
    {
        // 0b_000 - AND
        case 0: 
            *dst &= src;
            break;

        // 0b_001 - OR
        case 1: 
            *dst |= src;
            break;
        
        // 0b_010 - XOR
        case 2: 
            *dst ^= src;
            break;
        
        // 0b_011 - ADD
        case 3: 
            *dst += src;
            break;
        
        // 0b_100 - SUB
        case 4: 
            *dst -= src;
            break;
        
        // 0b_101 - INC
        case 5: 
            (*dst)++;
            break;

        // 0b_110 - DEC
        case 6: 
            (*dst)--;
            break;

        // 0b_111 - NOT
        case 7: 
            *dst = ~(*dst);
            break;
    }

}

unsigned int * mathOpDst() // Milan & Tabitha
{
    // returns the memory address of the destination
    unsigned int *dst = NULL;
    switch ((IR & MATH_DST_MASK) >> 2)
    {
        // 0b00 - Indirect (MAR used as a pointer)
        case 0:
            dst = (unsigned int *) &memory[MAR];
            break;
        
        // 0b01 - Accumlator ACC
        case 1:
            dst = (unsigned int *) &ACC;
            break;

        // 0b10 - Address Register MAR
        case 2: 
            dst = &MAR;
            break;

        // 0b11 - Memory Address
        case 3:
            dst = (unsigned int *) &memory[address()];
            break;
    }

    return dst;
}

unsigned int mathOpSrc() // Milan & Tabitha
{
    unsigned int src = 0;
    switch (IR & MATH_SRC_MASK)
    {
        // 0b00 - Indirect (MAR used as a pointer)
        case 0:
            src = memory[MAR];
            break;

        // 0b01 - Accumulator ACC
        case 1:
            src = ACC;
            break;

        // 0b10 - Constant
        case 2: 
            //if destination is MAR or Address
            if(IR & 0x08 == 0x08)
                src = address();
            else
                src = memory[PC - 1];
            break;

        // 0b11 - Memory
        case 3: 
            src = memory[address()];
            break;
    }

    return src;
}

void memOp() // Milan & Tabitha
{
    // 1 - LOAD
    if( (IR & MEM_FUNC_MASK) >> 3) 
    {
        // LOAD MAR
        if ((IR & MEM_REG_MASK) >> 2)
            MAR = (memory[memOpMeth()] << 8) + memory[memOpMeth() + 1];

        // LOAD ACC
        else
            ACC = memory[memOpMeth()];
    }

    // 0 - STOR
    else 
    {
        // STOR MAR
        if ((IR & MEM_REG_MASK) >> 2)
        {
            memory[memOpMeth()] = (MAR & 0xFF00) >> 8;
            memory[memOpMeth() + 1] = (MAR & 0x00FF);
        }


        // STOR ACC
        else
            memory[memOpMeth()] = ACC;
    }
}

unsigned int memOpMeth() // Milan & Tabitha
{
    unsigned int opAddress = 0;
    switch (IR & MEM_METH_MASK)
    {
        // 0b_000
        case 0: 
        //Operand is used as address
            opAddress = address();
            break;
        
        // 0b_001
        case 1: 
        //Operand is used as constant
            // if MAR
            if((IR & MEM_REG_MASK) >> 2)
                opAddress = PC - 2; //constant is 2 byte
            // if ACC
            else
                opAddress = PC - 1; //constant is 1 bytes
            break;

        // 0b_010
        case 2: 
        //Indirect(MAR used as pointer)
            opAddress = MAR;
            break;
    }

    return opAddress;
}

void branch() //Milan & Tabitha
{
    bool brch = false;
    switch(IR & BRANCH_TYPE_MASK)
    {
        case 0: // 0b000 - BRA
            brch = true;
            break;

        case 1: // 0b001 - BRZ
            if (ACC == 0 ) brch = true;
            break;

        case 2: // 0b010 - BNE
            if (ACC != 0 ) brch = true;
            break;

        case 3: // 0b011 - BLT
            if (ACC < 0 ) brch = true;
             break;

        case 4: // 0b100 - BLE
            if (ACC <= 0 )brch = true;
            break;
 
        case 5: // 0b101 - BGT
            if (ACC > 0 ) brch = true;
            break;

        case 6: // 0b110 - BGE
            if (ACC >= 0) brch = true;
            break;
    }

    //Branch
    if (brch)
        PC = address();
}