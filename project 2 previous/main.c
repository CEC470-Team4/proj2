#include <stdio.h>
#include <stdbool.h>
//#include <stdlib.h>
//#include <string.h>

#define HALT_OPCODE 0x19

// Written by Maxwell Moolchan and Megan Krajewski:
void fetchNextInstruction(void);
void executeInstruction(void);

// Written by Megan Krajewski:
unsigned int address();
unsigned int getOperandAddress();
unsigned int *getDestination();
unsigned int getSource();

// Written by Maxwell Moolchan:
//void initializeFromFile(void);
//void writeToFile(void);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;


int main(int argc, char* argv[]) {
    return 0;
}

// determines the location of the next instruction and increments the program counter accordingly
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

// decodes the opcode and performs the requested operation
void executeInstruction() {
    // Check if it's a math function
    if ((IR & 0x80) == 0x80) {
        // Check function
        switch(IR & 0x70){
            case 0x00: //AND
                *(getDestination()) &= getSource();
                break;
            case 0x10: //OR
                *(getDestination()) |= getSource();
                break;
            case 0x20: //XOR
                *(getDestination()) ^= getSource();
                break;
            case 0x30: //ADD
                *(getDestination()) += getSource();
                break;
            case 0x40: //SUB
                *(getDestination()) -= getSource();
                break;
            case 0x50: //INC
                (*(getDestination()))++;
                break;
            case 0x60: //DEC
                (*(getDestination()))--;
                break;
            case 0x70: //NOT
                *(getDestination()) = !*(getDestination());
                break;
        }
    }
    // Check if this is a memory function
    else if ((IR & 0xf0) == 0) {
        switch(IR & 0x0c){
            case 0x00: //STOR ACC
                memory[getOperandAddress()] = ACC;
                break;
            case 0x04: //STOR MAR
                memory[getOperandAddress()] = (MAR & 0xff00)>>8;
                memory[getOperandAddress()+1] = (MAR & 0x00ff);
                break;
            case 0x08: //LOAD ACC
                ACC = memory[getOperandAddress()];
                break;
            case 0x0c: //LOAD MAR
                MAR = (memory[getOperandAddress()]<<8) + memory[getOperandAddress()+1];
                break;
        }
    }
    // Check branch function
    else if ((IR & 0xF8) == 0x10) {
        bool branch = false;

        switch (IR & 0x07) {
            case 0: // Unconditional branch -- Load PC with (memory[pc+1] << 8) + memory[pc+2]
                branch = true;
                break;
            case 1: // Branch if ACC=0
                if(ACC = 0) branch = true;
                break;
            case 2: // Branch if ACC!=0
                if(ACC != 0) branch = true;
                break;
            case 3: // Branch if ACC<0
                if(ACC < 0) branch = true;
                break;
            case 4: // Branch if ACC<=0
                if(ACC <= 0) branch = true;
                break;
            case 5: // Branch if ACC>0
                if(ACC > 0) branch = true;
                break;
            case 6: // Branch if ACC>=0
                if(ACC >= 0) branch = true;
                break;
        }

        if(branch){
            PC = address();
        }
    }
    // Otherwise, it's a special opcode or an illegal opcode
    else {
        // The following is the logic code for a special opcode or an illegal opcode
        if(IR == 0x18) { // Then it's a NOP -- do nothing

        } else if(IR == 0x19) { // Then it's a HALT -- framework will halt for us

        } else { // Otherwise it's an illegal opcode -- you can print an error message if you want to

        }
    }
    printf("exit execute\n");
}

// a helper function that retrieves the 2 bytes after the opcode
unsigned int address(){
    return (memory[PC-2]<<8)+memory[PC-1];
}

// finds and returns the address of the operand for memory operations
unsigned int getOperandAddress() {
    unsigned int opAddress = 100000;

    switch(IR & 0x03){
        case 0: //operand is used as address
            opAddress = address();
            break;
        case 1: //operand is used as constant
            //if register is ACC
            if((IR & 0x07)==0x01){
                opAddress = PC-1; //constant is 1 byte
            } else{
                opAddress = PC-2; //constant is 2 bytes
            }
            break;
        case 2: //indirect(MAR used as pointer)
            opAddress = MAR;
            break;
    }

    return opAddress;
}

// finds and returns a pointer to the destination for mathematical operations
unsigned int *getDestination(){
    unsigned int *dest = NULL;
    switch(IR & 0x0c){
        case 0x00: //indirect(MAR used as pointer)
            dest = (unsigned int *)&memory[MAR];
            break;
        case 0x04: //ACC
            dest = (unsigned int *)&ACC;
            break;
        case 0x08: //MAR
            dest = &MAR;
            break;
        case 0x0c: //memory
            dest = (unsigned int *)&memory[address()];
            break;
    }
    return dest;
}

// finds and returns the source for mathematical operations
unsigned int getSource(){
    unsigned int source = 100000;
    switch(IR & 0x03){
        case 0: //indirect(MAR used as pointer)
            source = memory[MAR];
            break;
        case 1: //ACC
            source = ACC;
            break;
        case 2: //constant
            //if destination is MAR or an address
            if((IR & 0x08)==0x08){
                source = address(); //constant is 2 bytes
            } else{
                source = memory[PC-1]; //constant is 1 byte
            }
            break;
        case 3: //memory
            source = memory[address()];
            break;
    }
    return source;
}

/*

// takes the input text file, mem_in.txt, and puts it into the memory array
void initializeFromFile(void) {
    FILE *file;
    char row[MAXCHAR];
    char *token;
    int memIndex = 0;

    file = fopen("mem_in.txt", "r"); // Change the filename to whatever the input file is called

    if(file == NULL) { exit(2); }

    while(1) {
        fgets(row, MAXCHAR, file);
        //printf("Row: %s", row); // WAS USED TO PRINT OUT EACH ROW BEFORE PROCESSING, NO LONGER NEEDED

        token = strtok(row, "\n"); // REMOVE THE NEW LINE CHARACTER
        token = strtok(token, " "); // REMOVE THE SPACE


        while(token != NULL) {
            //printf("%s\n",token);
            memory[memIndex++] = (unsigned int) strtol(token, NULL, 16);

            token = strtok(NULL, " ");
        }
        if(feof(file)) { // if at the end of the file, break out of loop as there is nothing more
            break;
        }
    }
    fclose(file);
}

// takes the memory array and outputs it to a file, mem_out.txt
void writeToFile(void) {
    FILE *file;
    file = fopen("mem_out.txt","w");

    if(file == NULL) {
        printf("Error Writing to File!");
        exit(3);
    }

    int memorySize = sizeof(memory) / sizeof(memory[0]);
    for(int i = 0; i < memorySize; i++) {
        fprintf(file,"%02x ",memory[i]);
        if (i % 16 == 15 && i > 1) {
            fprintf(file,"%s","\n");
        }
    }

    fclose(file);
}
*/
