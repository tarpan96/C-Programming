/*
Tarpan Patel
Project 2
CIS 205 -T9
4/11/2017
This project is Simpletron Machine Language. The user can load memory and then runProgram and 
dump memory. 
*/

#include <stdio.h>
#include <stdlib.h>

#define SENTINEL -999999
#define READ 10
#define WRITE 11
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBSTRACT 31
#define MULTIPLY 32
#define DIVIDE 33
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43 

//Function Prototypes
void welcome();
void menu(int*);
void loadMemory(int* memory, int*, int);
void runProgram(int* memory, int* iR, int* opC, int* op, int* aC, int* pC, int);
void dumpMemory(int* memory, int* ac, int* pC, int* iR, int* opC, int* op, int);


int main()
{
	int instruction = 0;  //instructions
	int iRegister = 0;    //instruction register
	int opCode = 0;       //operation Code
	int operand = 0;      //operand 
	int accumulator = 0;  //accumulator
	int pCounter = 0;	  //Program Counter
	int MemorySize = 0;         //Memory size
	int option = 0;
			
	//Prompt user to enter the size of memory
	puts("how much memory do you want dynamically allocate?");
	scanf("%d", &MemorySize);
	
	//prompt user if the memory is greater or less mininum and maximum
	while(MemorySize  < 10 || MemorySize  > 1000)
	{
		puts("You requested too less or too much memory");
		puts("Enter memory size");
		scanf("%d", &MemorySize );
	}
	
	//Calloc memory
	int* Memory = (int *)calloc(MemorySize, sizeof(int));

	//Loop the menu until user quits or 
	do
	{	
		//Call menu function, with address of option passed through as parameters
		menu(&option);
	
		//execute based on users input
		switch(option)
		{
			//Case 1 calls welcome and loadMemory Function
			case 1: 
				welcome();
				loadMemory(Memory, &instruction, MemorySize);
			break;
			
			//Case 2 calls runProgram Function
			case 2:
				puts("*** Program excution begins ***");
				runProgram(Memory, &iRegister, &opCode, &operand, &accumulator, &pCounter, MemorySize);
			break;
			
			//Case 3, calls dumpMemory Function
			case 3:
				dumpMemory(Memory, &accumulator, &pCounter, &iRegister, &opCode, &operand, MemorySize);
			break;
			
			//Case 4 frees the memory
			case 4:
				free(Memory);
				puts("memory is released");
			break;
		}
	}while(option != 5);
}

//Welcome Function with intructions to loading memory
void welcome()
{
	puts("*** Welcome to Simpletron! ***");
	puts("*** Please enter your program one instruction ***");
	puts("*** (or data word) at a time. I will type the ***");
	puts("*** location number and a question mark (?).  ***");
	puts("*** You then type the word for that location. ***");
	puts("*** Type the sentinel -999999 to stop entering ***");
	puts("*** your program ***");
	puts("");
}

//Menu Function that displays menu and read users input 
void menu(int* op)
{	
		//Prompt user to enter an option
		
		puts("");
		puts("Enter 1 to load a program into memory");
		puts("Enter 2 to run the program in memory");
		puts("Enter 3 to dump the memory");
		puts("Enter 4 to free memory");
		puts("Enter 5 to quit");
		puts("");	
		
		//Read users choice
		scanf("%d", &*op);
		
}

//loadMemory Function with pointers to memory, instructions and size passed through parameters
void loadMemory(int* memory, int* instructions, int MemorySize) 
{	

	//Loop through MemorySize
	for(int i = 0; i < MemorySize; i++)
	{
		//Prompt user to enter word
		printf(">%03d? ", i);
		scanf("%d", &*instructions);

	//Check if users input is sentinel. Stop prompting user
	if(*instructions == SENTINEL)
	{
		//Set i to MemorySize, to stop looping
		i = MemorySize; 
		*instructions = 0;
	}
	//If input not sentinel, store it memory 
	else
	{
		memory[i] = *instructions;
	}

	}
	puts("*** Program loading completed ***");
	puts("");
}

//runProgram Function with pointers to memory, accumulator, programCounter, instructionRegister
//operationCode, operand, MemorySize as parameters
void runProgram(int* memory, int* iR, int* opC, int* op, int* ac, int* pC, int MemorySize )
{	
	int readInput = 0;
	
	//Loop through memory
	for(int i = 0; i < MemorySize; i++)
	{
		//assign i value to ProgramCounter(pC)
		*pC = i;		
		
		//Store the word in instructionRegister(iR)
		*iR = memory[*pC];
		
		//Store the word in operationCode(opC)
		*opC = *iR / 1000;
		
		//Store the memory address in operand(op)   
		*op =  *iR % 1000;
		
		//switch executes based on operationCode (opC)
		switch(*opC)
		{
			//READ(10) case, prompt user for input and store it in memory
			//operand location
			case READ: 
				puts("Enter an integer");
				scanf("%d", &memory[*op]);
			break;
				
			//WRITE(11) case, print the value stored in operand(op) location
			case WRITE:
				printf("The value stored is +%05d\n", memory[*op]);
			break;
				
			//LOAD(20) case, store the value in accumulator(ac) from memory operand(op) location
			case LOAD:
				*ac = memory[*op];
			break;
			
			//STORE(21) case, store the accumulator(ac) value in operand(op) location in memory
			case STORE:
				memory[*op] = *ac;
			break;
			
			//ADD(30) case, add the value in memory operand location to accumulator(ac)
			//Store the result in accumulator(ac)
			case ADD:
				*ac+= memory[*op];
			break; 
			
			//SUBSTRACT(31) case, subtract the value in memory operand(op) location from accumulator(ac)
			//Store the result in accumulator(ac)
			case SUBSTRACT:
				*ac-= memory[*op];
			break;
			
			//MULTIPLY(32) case, Calulate the product of the value stored in memory operand(op) location
			//and the accumulator(ac). Store the result in accumulator(ac)
			case MULTIPLY:
				*ac*= memory[*op];
			break;
			
			//DIVIDE(33) case, Find the quotient of accumulator and the value in memory operand(op) location
			//If the value in operand(op) location is 0, terminate the loop, else calculate the result
			case DIVIDE:
				if(memory[*op] != 0)
				{
					*ac/= memory[*op];
				}
				else
				{
					i = MemorySize;
					puts("Attempt to divide by zero");
					puts("Simpltron execution adnormally terminated");
				}
			break;
			
			//BRANCH(40) case, Set i to the operand(op) location and decrement i by 1
			case BRANCH:
				i = *op;				
				i--;
			break;
			
			//BRANCHNEG(41) case, if accumulator is less than 0,
			//set i to the operand(op) location and decrement i by 1
			case BRANCHNEG:
				if(*ac < 0)
				{
					i = *op;
					i--;
				}
			break;
			
			//BRANCHNEG(42) case, if accumulator is equal to 0,
			////set i to the operand(op) location and decrement i by 1
			case BRANCHZERO:
				if(*ac == 0)
				{
					i = *op;
					i--;
				}
			break;
			
			//HALT(43) case, End the loop, set i to size of Memory
			case HALT:
				i = MemorySize;
				puts("");
				puts("*** Simpletron execution terminated ***");
			break;
		}	
	}
}

//DumpMemory Function with pointers to memory, accumulator, programCounter, instructionRegister
//operationCode, operand, MemorySize as parameters
void dumpMemory(int* memory, int* aC, int* pC, int* iR, int* opC, int* op, int MemorySize)
{
	int i = 0, j = 0;
	
	//Print the accumulator(ac), instructionCounter(pC), instructionRegister(iR),
	//Print the operationCode(opC), operand(op)
	puts("");
	puts("REGISTERS:");
	printf("accumulator %15d\n", *aC);
	printf("instructionCounter %8d\n", *pC);
	printf("instructionRegister %7d\n", *iR);
	printf("operationCode %15d\n", *opC);
	printf("operand %19d\n", *op);
	puts("");

	puts("MEMORY");
	puts("");
	
	for(i = 0; i < 10; i++)
	{
		printf("%10d", i);
	}
	
	puts("");
	
	for(i = 0; i < MemorySize; i+=10)
	{
		printf("%02d ", i);
		
		for(j = i; j < i+10; j++)
		{
			printf("+%05d    ", memory[j]);
		}
		puts("");
	}
}	