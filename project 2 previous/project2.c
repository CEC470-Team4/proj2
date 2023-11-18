#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

void fetchNextInstruction(void);
void executeInstruction(void);
void determineOperation(unsigned char *);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;
char * memPtr;

int main(int argc, char * argv[]) {
    // Execution loop. Continue fetching and executing until PC
    // points to a HALT instruction
    FILE * fp;
    fp = fopen("mem_in.txt", "r");
    fgets(memPtr, 65536, fp);

    fprintf(fp, "%s\n");

    IR = memory[PC];
    PC = PC + memory[PC];
    determineOperation(memory);
    /*while(memory[PC] != HALT_OPCODE) {
        fetchNextInstruction();
        executeInstruction();
    }*/
    fclose(fp);
    return 0;
}

void determineOperation(unsigned char * input)
{
    // Check if it's a math function
    if ((IR & 0x80) == 0x80)
    {
        switch (IR & 0x70)
        {
            case 0x00:
                printf("AND");
                break;
            case 0x10:
                printf("OR");
                break;
            case 0x20:
                printf("XOR");
                break;
            case 0x30:
                printf("ADD");
                break;
            case 0x40:
                printf("SUB");
                break;
            case 0x50:
                printf("INC");
                break;
            case 0x60:
                printf("DEC");
                break;
            case 0x70:
                printf("NOT");
                break;
            default:
                printf("ERROR");
                break;
        }
        /*if ((IR & 0x70) == 0x00)    //AND
        {}
        else if((IR & 0x70) == 0x10)//OR
        {}
        else if((IR & 0x70) == 0x20)//XOR
        {}
        else if((IR & 0x70) == 0x30)//ADD
        {}
        else if((IR & 0x70) == 0x40)//SUB
        {}
        else if((IR & 0x70) == 0x50)//INC
        {}
        else if((IR & 0x70) == 0x60)//DEC
        {}
        else if((IR & 0x70) == 0x70)//NOT
        {}*/
    }
    // Check if this is a memory function
    else if ((IR & 0xf0) == 0)
    {
    }
    // Check branch function
    else if ((IR & 0xF8) == 0x10)
    {
    }
    // Otherwise, it's a special opcode or an illegal opcode
    else
    {
    }
}
// ================================
// Mathematical Operations:
// ================================

// If MSB == 1, the opcode is mathematical

// 1xxxyyzz

// where x is Function
// 000 = AND
// 001 = OR
// 010 = XOR
// 011 = ADD
// 100 = SUB
// 101 = INC
// 110 = DEC
// 111 = NOT

// where y is destination
// 00 = Indirect (MAR used as a pointer)
// 01 = Accumulator ACC
// 10 = Address register MAR
// 11 = Memory (The memory address is passed to the processor as a 16-bit operand following the opcode)

// where z is source
// 00 = Indirect (MAR used as a pointer)
// 01 = Accumulator ACC
// 10 = Constant (The constant is passed to the processor as an 8- or 16-bit operand following the opcode)
// 11 = Memory (The memory address is passed to the processor as a 16-bit operand following the opcode)

// ================================
// Memory Operations:
// ================================

// If the 4 most MSBs are 0000 then opcode represents memory or data transfer
// When the Method bit is equal to 00, the opcode is followed by a 16-bit operand
// that serves as the memory address.
// When the Method bit is equal to 01, the opcode is
// followed by an operand that serves as a constant (8 bits for ACC and 16 bits for MAR).
// Otherwise, the value contained in index register MAR is used as the memory address.

// 0000xyzz

// where x is function
// 0 = Store (STOR)
// 1 = Load (LOAD)

// where y is register
// 0 = Accumulator ACC
// 1 = Index register (MAR)

// where z is Method
// 00 = Operand is used as address
// 01 = Operand is used as a constant
// 10 = Indirect (MAR used as a pointer)

// ================================
// Branches/Jumps Operations:
// ================================

// If the most significant five bits are 00010, then the opcode represents an unconditional or
// conditional branch or jump. The opcode is always followed by a 16-bit operand that serves as
// the memory address.

// 00010xxx

// where x is type of branch
// 000 = BRA (Unconditional branch/branch always)
// 001 = BRZ (Branch if ACC = 0)
// 010 = BNE (Branch if ACC != 0)
// 011 = BLT (Branch if ACC < 0)
// 100 = BLE (Branch if ACC <= 0)
// 101 = BGT (Branch if ACC > 0)
// 110 = BGE (Branch if ACC >= 0)

// ================================
// Special Opcodes:
// ================================

// 00011000 = NOP (No operation. fetchNextInstruction() should increment PC by one to next instruction)
// 00011001 = Halt (Stops processor)

// ================================
// Other shit possibly needed:
// ================================

// ==== Hex to Bin converter ====
// if you look at the test txt file she gives it's all in hex and to read the instructions
// we have to convert to bin then read what it is

// ==== Some type of input ====
// like we need to take in the txt file values or any values given then do the stuff
// what do you think ???

// after the stuff is converted from hex to bin I assume we can use bit masks to
// check the first bits