// Authors: Milan Haruyama, Maegan Lucas, Tabitha O'Malley (Team 4)
// Professor: Dr. Laxima Niure Kandel
// Course: CEC 470
// Overview: Two-stage instruction decoder written in C.
//           Fetching written by Maegan Lucas.
//           Execution written by Milan Haruyama and Tabitha O'Malley.

#include <stdlib.h>
#include <stdio.h>

#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

#define MATH_OPCODE 0x80
#define MATH_FUNC 0x70
#define MATH_SRC 0x03
#define MATH_DST 0x0C

#define MEM_OPCODE 0x00
#define MEM_FUNC 0x08
#define MEM_REG 0x04
#define MEM_METH 0x03

#define BRANCH_OPCODE 0x10
#define BRANCH_TYPE 0x07

FILE *fileptr;

unsigned char memory[65536];
unsigned char ACC = 0x00;
unsigned char IR = 0x00;
unsigned int MAR = 0x0000;
unsigned int PC = 0x0000;

void fetchNextInstruction(void);
void executeInstruction(void);

unsigned int address(void);

void mathOp(void);
unsigned int mathOpSrc(void);
unsigned int * mathOpDst(void);

void memOp(void);
unsigned int memOpReg(void);
unsigned int memOpMeth(void);

void branch(void);
void branch_func(void);

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
    unsigned char c1, c2;
    while(!feof(fileptr))
    {
        fscanf(fileptr, "%d", &memory[i]);
        printf("%d\n", memory[i]);
        i++;
    }

    fclose(fileptr);
}

void fetchNextInstruction(void) // Maegan
{


}

void executeInstruction(void) // Milan and Tabitha
{
    // check for regular opcodes first //

    // Mathematical operations
    if ((IR & MATH_OPCODE) == MATH_OPCODE)
        mathOp();

    // Memory operations
    else if ((IR & MEM_OPCODE) == MEM_OPCODE)
        memOpReg();

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
            printf("Stopping!\n"); 
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
        case 0: // 0b_000 - AND
            *dst &= src;
            break;

        case 1: // 0b_001 - OR
            *dst |= src;
            break;
        
        case 2: // 0b_010 - XOR
            *dst ^= src;
            break;
        
        case 3: // 0b_011 - ADD
            *dst += src;
            break;
        
        case 4: // 0b_100 - SUB
            *dst -= src;
            break;
        
        case 5: // 0b_101 - INC
            (*dst)++;
            break;
        
        case 6: // 0b_110 - DEC
            (*dst)--;
            break;

        case 7: // 0b_111 - NOT
            *dst = ~(*dst);
            break;
    }

}

unsigned int mathOpSrc() // Milan
{
    unsigned int src = 0;
    switch ((IR & MATH_SRC))
    {
        case 0:
        //Indirect (MAR used as a pointer)
        src = memory[MAR];
        break;

        case 1:
        //Accumulator ACC
        src = ACC;
        break;

        case 2: 
        //Constant
        // src = ?;
        break;

        case 3: 
        //Memory
        // src = memory[];
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

        case 2: 
        //Address register MAR
        dst = &MAR;
        break;

        case 3:
        //Memory
        dst = (unsigned int *)&memory[address()];
        break;
    }

    return dst;
}



void memOp()
{
    unsigned int meth = memOpMeth();
    unsigned int reg = memOpReg();

    if( (IR & MEM_FUNC) >> 3) // 1 - LOAD
    {

    }

    else // 0 - STOR
    {

    }
}

unsigned int memOpMeth()
{
    unsigned int opAddress = 0;

    switch (IR & MEM_METH)
    {
        case 0: // 0b_000
        //Operand is used as address
        opAddress = address();        
        break;
        
        case 1: // 0b_001
        //Operand is used as constant
        if(memOpReg)
        {

        }
        else
        {

        }

        break;

        case 2: // 0b_010
        //Indirect(MAR used as pointer)

        break;
    }

    return opAddress;
}

unsigned int memOpReg() // Tabitha & Milan
{
    //returns a 1 or 0 depending on which register is used
    // 0 - Accumulator ACC
    // 1 - Index Register MAR

     return ((IR & MEM_REG) >> 2);
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
    if (brch){
        PC = address();    
    }
    
}