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

// Special Opcode Masks
#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

// Math Opcode Masks
#define MATH_OPCODE 0x80
#define MATH_FUNC 0x70
#define MATH_SRC 0x03
#define MATH_DST 0x0C

// Memory Opcode Masks
#define MEM_OPCODE 0x00
#define MEM_FUNC 0x08
#define MEM_REG 0x04
#define MEM_METH 0x03

// Branch Opcode Masks
#define BRANCH_OPCODE 0x10
#define BRANCH_TYPE 0x07

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
unsigned int mathOpSrc(void);
unsigned int * mathOpDst(void);

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

// void fetchNextInstruction(void) // Maegan
// {
//     int bytes = 0;
//     IR = memory[PC];
//     if((IR & MATH_OPCODE) == MATH_OPCODE) 
//     {
//         if ((IR & MATH_FUNC) >> 4) 
//         {
//             if ((IR & MATH_DST) == MATH_DST)
//                 bytes = 2;
//             else
//                 bytes = 1;
//         } 
//         else if (((IR & MATH_DST) != MATH_DST) && (IR & MATH_DST) >> 2 == 0)
//             bytes = 1;
//         else if ((IR & MATH_DST) >> 2 == 2)
//             bytes = 2;
//         else
//             bytes = 3;
//     } 
//     else if ((IR & MEM_OPCODE) == MEM_OPCODE) 
//     {
//         if ((IR & MEM_METH) == 1) {
//             if ((IR & MEM_REG) >> 2) {
//                 bytes = 2;
//             } else {
//                 bytes = 1;
//             }
//         } 
//         else
//             bytes = 3;
//     } 
//     else if ((IR & BRANCH_OPCODE) == BRANCH_OPCODE)
//         bytes = 3;
//     else if (IR == NOP_OPCODE)
//         bytes = 1;

//     PC += bytes;
// }

void fetchNextInstruction() {
    IR = memory[PC];
    int bytes = 0;

    // Check if it's a math function
    if ((IR & 0x80) == 0x80) {
        //function is INC, DEC, or NOT
        if((IR & 0x70)>0x40){
            //destination is an address
            if((IR & 0x0c)==0x0c){
                bytes = 2;

            //destination is *MAR, ACC, or MAR
            } else{
                bytes = 1;
            }
        //destination is not an address AND source is *MAR or ACC
        } else if(((IR & 0x0c) != 0x0c) && ((IR & 0x02) == 0)){
            bytes = 1;

        //destination is *MAR or ACC AND source is a constant
        } else if((IR & 0x0b) == 2){
            bytes = 2;

        //destination OR source are an address
        } else{
            bytes = 3;
        }
    }
    // Check if this is a memory function
    else if ((IR & 0xf0) == 0) {
        //method is *MAR
        if((IR & 0x02) == 2){
            bytes = 1;

        //register is ACC AND method is a constant
        } else if((IR & 0x07) == 1){
            bytes = 2;

        //else, 3 bytes
        } else{
            bytes = 3;
        }
    }
    // Check branch function
    else if ((IR & 0xf8) == 0x10) {
        bytes = 3;
    }
    // Otherwise, it's a special opcode or an illegal opcode
    else {
        bytes = 1;
    }

    PC += bytes;
}

void executeInstruction(void) // Milan and Tabitha
{
    // check for regular opcodes first //

    // Mathematical operations
    if ((IR & MATH_OPCODE) == MATH_OPCODE)
        mathOp();

    // Memory operations
    else if ((IR & MEM_OPCODE) == MEM_OPCODE)
        memOp();

    // Branches/Jumps
    else if ((IR & BRANCH_OPCODE) == BRANCH_OPCODE)
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

unsigned int address()
{
    // retrieves the two byte address proceeding the opcode
    // also converts the address from big-endian to little-endian
    return (memory[PC - 2] << 8) + memory[PC - 1];
}

void mathOp()
{
    unsigned int src = mathOpSrc();
    unsigned int * dst = mathOpDst();

    switch((IR & MATH_FUNC) >> 4)
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

unsigned int mathOpSrc() // Milan
{
    unsigned int src = 0;
    switch ((IR & MATH_SRC))
    {
        //Indirect (MAR used as a pointer)
        case 0:
            src = memory[MAR];
            break;

        //Accumulator ACC
        case 1:
            src = ACC;
            break;

        //Constant
        case 2: 
            //if destination is MAR or Address
            if((IR & 0x08) == 0x08)
                src = address();
            else
                src = memory[PC - 1];
            break;

        //Memory
        case 3: 
            src = memory[address()];
            break;
    }

    return src;
}

unsigned int * mathOpDst() // Milan
{
    // returns the memory address of the destination
    unsigned int *dst = NULL;
    switch ((IR & MATH_DST) >> 2)
    {
        case 0:
        //Indirect (MAR used as a pointer)
        dst = (unsigned int *) &memory[MAR];
        break;
        
        case 1:
        //Accumlator ACC
        dst = (unsigned int *) &ACC;
        break;

        //Address register MAR
        case 2: 
            dst = &MAR;
            break;

        //Memory
        case 3:
            dst = (unsigned int *) &memory[address()];
            break;
    }

    return dst;
}



void memOp()
{
    // 1 - LOAD
    if( (IR & MEM_FUNC) >> 3) 
    {
        // LOAD MAR
        if ((IR & MEM_REG) >> 2)
            MAR = (memory[memOpMeth()] << 8) + memory[memOpMeth() + 1];

        // LOAD ACC
        else
            ACC = memory[memOpMeth()];
    }

    // 0 - STOR
    else 
    {
        // STOR MAR
        if ((IR & MEM_REG) >> 2)
        {
            memory[memOpMeth()] = (MAR & 0xFF00) >> 8;
            memory[memOpMeth() + 1] = (MAR & 0x00FF);
        }


        // STOR ACC
        else
            memory[memOpMeth()] = ACC;
    }
}

unsigned int memOpMeth()
{
    unsigned int opAddress = 0;
    switch (IR & MEM_METH)
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
            if((IR & MEM_REG) >> 2)
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

void branch () //Milan and Tabitha
{
    bool brch = false;
    switch(IR & BRANCH_TYPE)
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