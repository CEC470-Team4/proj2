
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

void fetchNextInstruction(void);
void executeInstruction(void);
void initializeFromFile(void);
void outputToFile(void);

void fetchNextInstruction(void) {

    static int firstPass = 1;

    if(!firstPass == 1) {
        //initializeFromFile();
        if((IR & 0x80) == 0x80) { // if IR masked with 10000000 results in 1000 0000, Mathematical operation occours
            int function;
            int destination;
            int source;

            function = (IR & 0x70) >> 4; // we need to mask with 01110000
            destination = (IR & 0x0C) >> 2; // we need to mask with 00001100
            source = (IR & 0x03); // we need to mask with 00000011

            if(function == 5 || function == 6 || function == 7) {
                // if function is equal to any of the following PC is only incremented by 1 because the opcode doesn't need an operand
                // function is INC 101
                // function is DEC 110
                // function is NOT 111

                PC += 1;
            }
            else if(destination == 3 || source == 3) {
                PC += 3;
            }
            else if(destination == 1 || source == 1) {
                PC += 2;
            }
            else {
                PC += 3;
            }
        }
        else if((IR & 0xF0) == 0x00) { // if IR masked with 11110000 results in 0000 0000, Memory Operation occours
            int registerportion;
            int method;

            registerportion = (IR & 0x04) >> 2; // we need to mask with 00000100
            method = (IR & 0x03); // we need to mask with 00000011

            if (registerportion == 0 && method == 1) {
                PC += 2;
            }
            else {
                PC += 3;
            }


            /*
            if(method == 0 || method == 2) {
                PC += 3; // opcode followed by 16bit operand
            }
            else if(method == 1) { // opcode followed by operand that serves as constant (8 bits for ACC, 16 bits for MAR)
                if(registerportion == 0) { // Accumulator, 8 bits
                    PC += 2;
                }
                else if(registerportion == 1) { // MAR, 16 bits
                    PC += 3;
                }
            }*/
        }
        else if((IR & 0xF8) == 0x10) { // if IR masked with 11111000 results in 0001 0000, Branching operation occours
            PC += 3; // always followed by 16 bit operand
        }
        else if ((IR & 0xFF) == 0x18){ // if IR masked with 11111111 results in 0001 1000, Special opcode, no operation just increment PC by 1 to next instruction
            PC += 1;
        }

        else /*((IR & 0xFF) == 0x19)*/ { // if IR masked with 11111111 results in 0001 1001, Special opcode, Halt, Stops Processor
            // THIS IS COVERED BY THE MAIN WHILE LOOP, IT FINDS THE MAIN HALT CODE, NOT NEEDED Here
            outputToFile();
        }
    }

    firstPass = 0;
    IR = memory[PC];
}

void executeInstruction(void) { // DETERMINES WHICH OPERATION TO PREFORM
    if((IR & 0x80) == 0x80) { // if IR masked with 10000000 results in 1000 0000, Mathematical operation occours
        int functionSection;
        int destinationSection;
        int sourceSection;

        functionSection = (IR & 0x70) >> 4; // we need to mask with 01110000
        destinationSection = (IR & 0x0C) >> 2; // we need to mask with 00001100
        sourceSection = (IR & 0x03); // we need to mask with 00000011

        unsigned int address = (memory[PC +1] << 8) + memory[PC +2];
        unsigned int MBR = 0;
        unsigned int* destination = &memory[MAR];

        switch(sourceSection) {
        case 0: // indirect
            MBR = memory[MAR];
            break;
        case 1: // ACC
            MBR = ACC;
            break;
        case 2: // constant
            if(destinationSection != 1) {
                MBR = address;
            }
            else {
                MBR = memory[PC + 1];
            }
            break;
        case 3: // Memory
            MBR = memory[address];
            break;
        }

        switch(destinationSection) {
            case 0: // indirect
                destination = &memory[MAR];
                break;
            case 1: // ACC
                //printf("helloImHere"); // debugging
                destination = &ACC;
                break;
            case 2: // MAR
                destination = &MAR;
                break;
            case 3: // Memory
                destination = &memory[address];
                break;
        }

        switch(functionSection) {
            case 0: // AND
                *(destination) &= MBR;
                break;
            case 1: // OR
                *(destination) |= MBR;
                break;
            case 2: // XOR
                *(destination) ^= MBR;
                break;
            case 3: // ADD
                *(destination) += MBR;
                break;
            case 4: // SUB
                *(destination) -= MBR;
                break;
            case 5: // INC
                //printf("ya\n"); // debugging
                //printf("%x\n",*destination);
                (*destination)++;
                //printf("%x\n",*destination); // debugging
                break;
            case 6: // DEC
                (*destination)--;
                break;
            case 7: // NOT
                *(destination) =! *(destination);
                break;
        }

    }
    else if((IR & 0xF0) == 0x00) { // if IR masked with 11110000 results in 0000 0000, Memory Operation occours
        int functionSection;
        int registerSection;
        int methodSection;

        functionSection = (IR & 0x08) >> 3; // we need to mask with 00001000
        registerSection = (IR & 0x04) >> 2; // we need to mask with 00000100
        methodSection = (IR & 0x03); // we need to mask with 00000011

        if(functionSection == 0) { // STOR
            if(registerSection == 0) { // ACC
                if (methodSection == 0) { // Operand used as address
                    unsigned int address = (memory[PC +1] << 8) + memory[PC +2];
                    memory[address] = ACC;
                }
                else if (methodSection == 2) { // MAR used as pointer
                    memory[MAR] = ACC;
                }
            }
            else if(registerSection == 1) { // MAR
                if (methodSection == 0) { // Operand used as address
                    unsigned int address = (memory[PC +1] << 8) + memory[PC +2];
                    memory[address] = MAR;
                }
                else if (methodSection == 2) { // MAR used as pointer
                    memory[MAR] = MAR;
                }
            }
        }
        else if(functionSection == 1) { // LOAD
            //printf("hello");
            if(registerSection == 0) { // ACC
                if (methodSection == 0) { // Operand used as address
                    unsigned int address = (memory[PC +1] << 8) + memory[PC +2];
                    ACC = memory[address];
                }
                else if(methodSection ==1) { // Operand used as constant
                    ACC = memory[PC + 1];
                }
                else if (methodSection ==2) { // MAR used as pointer
                    ACC = memory[MAR];
                }
            }
            else if(registerSection == 1) { // MAR
                if (methodSection == 0) { // Operand used as address
                    unsigned int address = (memory[PC +1] << 8) + memory[PC +2];
                    MAR = memory[address];
                }
                else if(methodSection == 1) { // Operand used as constant
                    MAR = memory[PC + 1];
                }
                else if (methodSection == 2) { // MAR used as pointer
                    MAR = memory[MAR];
                }
            }
        }
    }
    else if((IR & 0xF8) == 0x10) { // if IR masked with 11111000 results in 00010 000, Branching operation occours
        int typeOfBranch = (IR & 3);
        unsigned int arg = (memory[PC +1] << 8) + memory[PC + 2];
        switch(typeOfBranch) {
            case 0:
                PC = arg;
                break;
            case 1:
                if(ACC == 0) {
                    PC = arg;
                }
            case 2:
                if(ACC != 0) {
                    PC = arg;
                }
            case 3:
                if(ACC < 0) {
                    PC = arg;
                }
            case 4:
                if(ACC <= 0) {
                    PC = arg;
                }
            case 5:
                if(ACC > 0) {
                    PC = arg;
                }
            case 6:
                if(ACC >= 0) {
                    PC = arg;
                }
        }
    }
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

void outputToFile()
{
    FILE *fp;
    char row[MAXCHAR];
    unsigned char *token;
    int mempos = 0;

    fp = fopen("output.txt", "w");

    /*for(int i = 0; i < sizeof(memory); i++)
    {
        //fputc(memory[mempos], fp);
        fputs("%s" ,fp);
        mempos++;
    }*/

    while(memory[mempos] != strlen(memory))
    {
       strcpy(row, memory);
       fprintf(fp, row);
       mempos++;
    }

    fclose(fp);
}

int main(int argc, char * argv[]) {
    // Execution loop. Continue fetching and executing until PC
    // points to a HALT instruction

    // Temporary, from the project instructions

    memory[0] = 0x08;
    memory[1] = 0x10;
    memory[2] = 0x00;
    memory[3] = 0xb7;
    memory[4] = 0x10;
    memory[5] = 0x01;
    memory[6] = 0xb7;
    memory[7] = 0x10;
    memory[8] = 0x02;
    memory[9] = 0xa6;
    memory[10] = 0xff;
    memory[11] = 0xd4;
    memory[12] = 0x00;
    memory[13] = 0x10;
    memory[14] = 0x03;
    memory[15] = 0x19;

    memory[0x1000] = 4;
    memory[0x1001] = 3;
    memory[0x1002] = 2;


    while(memory[PC] != HALT_OPCODE) {
        fetchNextInstruction();
        executeInstruction();

        //printf("PC = %d  \tACC = %x  \tMAR = %x  \tIR = %x  \tmemory[0x1003] = %x\n", PC, ACC, MAR, IR, memory[0x1003]);
    }
    outputToFile();
    //printf("%x",memory[0x1003]);
    return 0;
}
