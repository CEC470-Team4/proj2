// Authors: Milan Haruyama, Maegan Lucas, Tabitha O'Malley
// Professor: Dr. Laxima Niure Kandel
// 


#include <stdlib.h>
#include <stdio.h>

#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

void fetchNextInstruction(void);
void executeInstruction(void);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;

int main(int argc, char * argv[])
{
    // execution loop
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

void executeInstruction(void)
{
    if (IR == HALT_OPCODE)
    {
        // HALT
    }
    
    else if (IR == NOP_OPCODE)
    {

        // DO NOTHING
        
    }

    else

    switch(IR)
    {
        case 0: // 000 - BRA

            break;

        case 1: // 001 - BRZ

            break;

        case 2: // 010 - BNE

            break;

        case 3: // 011 - BLT

            break;

        case 4: // 100 - BLE

            break;
 
        case 5: // 101 - BGT

            break;

        case 6: // 110 - BGE

            break;
    }
}