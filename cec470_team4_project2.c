// Authors: Milan Haruyama, Maegan Lucas, Tabitha O'Malley
// Professor: Dr. Laxima Niure Kandel

#include <stdlib.h>
#include <stdio.h>

#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

void fetchNextInstruction(void);
void executeInstruction(void);
void branch(void);
void memOp(void);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;

unsigned int IR_branchmask = 0b11111000; // obtains the five MSBs of IR
unsigned int IR_methodmask = 0b11110000; ///obtains the five MSBs of IR


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
    else if (((IR & IR_branchmask) >> 3) == 2)
    {
        branch();
    }
    else if (((IR & IR_methodmask) >> 3) == 0)
    {
        memOp();
    }
    
}

void branch ()
{
    switch(IR)
    {
        case 0: // 0b000 - BRA
            //Branch
            break;

        case 1: // 0b001 - BRZ
            if (ACC == 0 ){
                //Branch
            }

            break;

        case 2: // 0b010 - BNE
            if (ACC != 0 ){
                //Branch
            }

            break;

        case 3: // 0b011 - BLT
            if (ACC < 0 ){
                //Branch
            }

            break;

        case 4: // 0b100 - BLE
            if (ACC <= 0 ){
                //Branch
            }

            break;
 
        case 5: // 0b101 - BGT
            if (ACC > 0 ){
                //Branch
            }
            break;

        case 6: // 0b110 - BGE
            if (ACC >= 0 ){
                //Branch
            }

            break;
    }


}

void memOp()
{
    
}