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

}

void executeInstruction(void) // Milan and Tabitha
{
    // check for regular opcodes first //

    // Mathematical operations
    if ((IR & MATH_OPCODE) == MATH_OPCODE)
        mathOp();

    // Memory operations
    else if (((IR & IR_mem_ops_mask) >> 3) == 0)
        memOpReg();

    // Branches/Jumps
    else if (((IR & IR_branch_mask) >> 3) == 2)
        branch();

    // check for HALT, NOP, or illegal opcodes last //
    else 
    switch(IR)
    {
        // HALT
        case HALT_OPCODE:
            // halts program
            memory[PC] = HALT_OPCODE; 
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