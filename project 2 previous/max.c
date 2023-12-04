
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HALT_OPCODE 0x19
#define MAXCHAR 1000

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;

void fetchNextInstruction(void) {




}
void executeInstruction(void) { // DETERMINES WHICH OPERATION TO PREFORM
    /*
    if((IR & 0x80) == 0x80) { // if IR masked with 10000000 results in 1000 0000, Mathematical operation occours

    }
    else if((IR & 0xF0) == 0x00) { // if IR masked with 11110000 results in 0000 0000, Memory Operation occours

    }
    else if((IR & 0xF8) == 0x10) { // if IR masked with 11111000 results in 00010 000, Branching operation occours

    }
    else if((IR & 0xFF) == 0x18) { // if IR masked with 11111111 results in 0001 1000, Special opcode, no operation just increment PC by 1 to next instruction

    }
    else if((IR & 0xFF) == 0x19) { // if IR masked with 11111111 results in 0001 1001, Special opcode, Halt, Stops Processor

    }
    else {
        exit(99); // CODE 99 ,UHHH dont think this happens ever but is here
    }

     */
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
// followed by an operand thatserves as a constant (8 bits for ACC and 16 bits for MAR).
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


}



void initializeFromFile(void) { // Written by Maxwell Moolchan
    FILE *file;
    char row[MAXCHAR];
    unsigned char *token;
    int mempos = 0;

    file = fopen("mem_in.txt", "r");

    if(file == NULL) { exit(2); }

    while(1) {
        fgets(row, MAXCHAR, file);
        //printf("Row: %s", row); // WAS USED TO PRINT OUT EACH ROW BEFORE PROCESSING, NO LONGER NEEDED

        token = strtok(row, "\n"); // REMOVE THE NEW LINE CHARACTER
        token = strtok(token, " "); // REMOVE THE SPACE

        while(token != NULL) {

            for(int i = 0; i < strlen(token); i++) {

                //printf("%c", token[i]);
                memory[mempos] = token[i];
                //printf("%d",mempos);
                mempos++;
            }

            token = strtok(NULL, " ");
        }
        if(feof(file)) { // if at the end of the file, break out of loop as there is nothing more
            break;
        }
    }
    fclose(file);

    for(int i = 0; i < strlen(memory); i++) {
        printf("%c",memory[i]);
    }
    //printf("\n%c", memory[strlen(memory) -1]);
    printf("\nLength of Memory Array: %d",strlen(memory));

}

int main(int argc, char * argv[]) {
    // Execution loop. Continue fetching and executing until PC
    // points to a HALT instruction

    initializeFromFile(); // THIS IS USED TO INITIALIZE THE MEMORY ARRAY FROM A GIVEN TXT FILE

    while(memory[PC] != HALT_OPCODE) {
        fetchNextInstruction();
        executeInstruction();

        //memory[PC] = HALT_OPCODE; // to exit the loop for now cuz we dont have the necessary crap to stop it


    }
    return 0;
}
