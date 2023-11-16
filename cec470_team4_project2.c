#include <stdlib.h>
#include <stdio.h>

#define HALT_OPCODE 0x19

void fetchNextInstruction(void);
void executeInstruction(void);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IC = 0;
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
        executeInstruction;
    }

    return 0;
}