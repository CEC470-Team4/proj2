// Authors: Milan Haruyama, Maegan Lucas, Tabitha O'Malley
// Professor: Dr. Laxima Niure Kandel

#include <stdlib.h>
#include <stdio.h>

#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

void fetchNextInstruction(void);
void executeInstruction(void);

void mathOp(void);
unsigned int mathOpSrc(void);
unsigned int mathOpDst(void);

void branch(void);
void branch_func(void);

void memOpReg(void);
void memOpMeth(void);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;

// Math operations
unsigned int IR_math_func_mask = 0b01110000;
unsigned int IR_math_dst_mask = 0b00001100;

// Memory operations
unsigned int IR_mem_ops_mask = 0b11110000; ///obtains the four MSBs of IR
unsigned int IR_mem_fuction_mask = 0b00001000;
unsigned int IR_mem_register_mask = 0b00000100;

// Branches/Jumps
unsigned int IR_branch_mask = 0b11111000; // obtains the five MSBs of IR
unsigned int IR_branch_type_mask = 0b00000111; // obtains the five MSBs of IR

// General purpose
unsigned int IR_2_lsb_mask = 0b00000011;



int main(int argc, char * argv[])
{
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

void fetchNextInstruction(void)
{

    PC += 1; // increment PC by blah blah blah
}

void executeInstruction(void) //Milan and Tabitha
{
    // check for regular opcodes first //

    // Mathematical operations
    if ((IR >> 7))
        mathOp();

    // Memory operations
    else if (((IR & IR_mem_ops_mask) >> 3) == 0)
        memOpReg();

    // Branches/Jumps
    else if (((IR & IR_branch_mask) >> 3) == 2)
        branch();

    // check for HALT or NOP last //
    else 
    switch(IR)
    {
        // HALT
        case HALT_OPCODE:
            memory[PC] = HALT_OPCODE;
            break;

        // NOP
        case NOP_OPCODE:
            fetchNextInstruction(); // GO TO NEXT INSTRUCTION
            break;
    }

}

void branch () //Milan and Tabitha
{
    switch(IR & IR_branch_type_mask)
    {
        case 0: // 0b000 - BRA
            break;

        case 1: // 0b001 - BRZ
            if (ACC == 0 )
            break;

        case 2: // 0b010 - BNE
            if (ACC != 0 )
            break;

        case 3: // 0b011 - BLT
            if (ACC < 0 )
            break;

        case 4: // 0b100 - BLE
            if (ACC <= 0 )
            break;
 
        case 5: // 0b101 - BGT
            if (ACC > 0 )
            break;

        case 6: // 0b110 - BGE
            if (ACC >= 0)
            break;

        branch_func();
    }


}

void branch_func()
{

}

void mathOp()
{
    unsigned int src = mathOpSrc();
    unsigned int dst = mathOpDst();

    switch((IR & IR_math_func_mask)>> 4)
    {
        case 0:
            dst = dst & src;
            break;

        case 1:
            dst = dst | src;
            break;
        
        case 2:
            dst = dst ^ src;
            break;
        
        case 3:
            dst = dst + src;
            break;
        
        case 4:
            dst = dst - src;
    }

    return dst;
}

unsigned int mathOpSrc() //Tabitha
{
    unsigned int src = 0;
    switch ((IR & IR_2_lsb_mask))
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

unsigned int mathOpDst() //Tabitha
{
    unsigned int dst = 0;
    switch ((IR & IR_math_dst_mask)>>2)
    {
        case 0:
        //Indirect (MAR used as a pointer)
        dst = memory[MAR];
        break;
        
        case 1:
        //Accumlator ACC

        break;

        case 2: 
        //Address register MAR
        dst = MAR;
        break;

        case 3:
        //Memory
        // dst = memory[];
        break;
    }

    return dst;
}

void memOpReg() //Tabitha
{
     if ((IR & IR_mem_register_mask)== 0)
     {
        //Accumulator ACC
     }
     else 
     {
        //Index register MAR
     }
    memOpMeth();
}

void memOpMeth()
{
    switch (IR & IR_2_lsb_mask)
    {
        case 0: 
        //Operand is used as address
        break;
        
        case 1:
        //Operand is used as constant
        break;

        case 2:
        //Indirect(MAR used as pointer)
        break;
    }


}