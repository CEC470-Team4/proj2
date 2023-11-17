// Authors: Milan Haruyama, Maegan Lucas, Tabitha O'Malley
// Professor: Dr. Laxima Niure Kandel

#include <stdlib.h>
#include <stdio.h>

#define HALT_OPCODE 0x19
#define NOP_OPCODE 0x18

void fetchNextInstruction(void);
void executeInstruction(void);
void branch(void);
void memOpReg(void);
void memOpMeth(void);
void MathOpFunction(void);
void MathOpDesination(void);
void MathOpSource(void);

unsigned char memory[65536];
unsigned char ACC = 0;
unsigned char IR = 0;
unsigned int MAR = 0;
unsigned int PC = 0;

unsigned int IR_branch_mask = 0b11111000; // obtains the five MSBs of IR
unsigned int IR_method_mask = 0b11110000; ///obtains the four MSBs of IR
unsigned int IR_mem_fuction_mask = 0b00001000;
unsigned int IR_mem_register_mask = 0b00000100;
unsigned int IR_2_LSB_mask = 0b00000011;
unsigned int IR_MSB_mask = 0b10000000;
unsigned int IR_Math_fuction_mask = 0b01110000;
unsigned int IR_Math_desintation_mask = 0b00001100;



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

void executeInstruction(void) //Milan and Tabitha
{
    // checks for HALT or NOP opcodes first
    if (IR == HALT_OPCODE)
    {
        // HALT
        memory[PC] = HALT_OPCODE;
    }
    
    else if (IR == NOP_OPCODE)
    {
        // GO TO NEXT INSTRUCTION
        PC++;
    }

    // checks for the rest

    else if (((IR & IR_branch_mask) >> 3) == 2)
    {
        branch();
    }

    else if (((IR & IR_method_mask) >> 3) == 0)
    {
        if ((IR & IR_mem_fuction_mask) == 0)
        {
            //Store
            memOpReg;
        }
         else 
        {
            //Load
            memOpReg;
        }
    }
    else if (((IR & IR_MSB_mask)>> 7) == 1)
    {

    }
}

void branch () //Milan and Tabitha
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


void memOpReg() //Tabitha
{
     if ((IR & IR_mem_register_mask)== 0){
        //Accumulator ACC
        memOpMeth();
     }
     else {
        //Index register MAR
        memOpMeth();
     }
}

void memOpMeth()
{
    switch (IR & IR_2_LSB_mask)
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

void MathOpFunction() //Tabitha
{
    switch((IR & IR_Math_fuction_mask)>> 4)
    {
        case 0:
        //AND
        MathOpDestination();
        break;

        case 1:
        //OR
        MathOpDestination();
        break;

        case 2:
        //XOR
        MathOpDestination();
        break; 

        case 3:
        //ADD
        MathOpDestination();
        break;

        case 4:
        //SUB
        MathOpDestination();
        break;

        case 5:
        //INC
        MathOpDestination();
        break;

        case 6:
        //DEC
        MathOpDestination();
        break;

        case 7:
        //NOT
        MathOpDestination();
        break;
    }
}

void MathOpDestination() //Tabitha
{
    switch ((IR & IR_Math_desintation_mask)>>2)
    {
        case 0:
        //Indirect (MAR used as a pointer)
        MathOpSource();
        break;
        
        case 1:
        //Accumlator ACC
        MathOpSource();
        break;

        case 2: 
        //Address register MAR
        MathOpSource();
        break;

        case 3:
        //Memory
        MathOpSource();
        break;

    }
}

void MathOpSource() //Tabitha
{
    switch ((IR & IR_2_LSB_mask))
    {
        case 0:
        //Indirect (MAR used as a pointer)
        break;

        case 1:
        //Accumulator ACC
        break;

        case 2: 
        //Constant 
        break;

        case 3: 
        //Memory
        break;
    }

}