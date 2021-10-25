/*
Tarpan Patel 
Project3 - 1a
CIS 205 -T9
5/4/2017
This program takes in an text file and encrypts the file and outputs the result in an different 
file. The program takes arguments from the command line.
*/

#include <stdio.h>
#include <stdlib.h>

//define typedef for unsigned char
typedef unsigned char BYTE;


int main(int argc, char** argv)
{
	//pointer to each byte in file
	FILE* cfPtr;
	
	//Open the file and read each byte 
	if((cfPtr = fopen(argv[1], "rb+")) == NULL)
	{
		//print if no file exists
		puts("No file found");
	}
	else
	{
		//initialize variables
		BYTE mask = 192;
		BYTE result;
		BYTE chByte;
		int fSize;
		
		//Find the size of the file by moving pointer to the EOF
		fseek(cfPtr, 0, SEEK_END);
		
		//assign the file size to fSize
		fSize = ftell(cfPtr);
		
		//Set pointer back to the beginning 
		fseek(cfPtr, 0, SEEK_SET);
	
		//Another pointer to write each byte to file
		FILE* encrypted;
		
		//create fileEncrypted.txt file and write in it
		encrypted = (fopen("fileEncrypted.txt", "wb+"));
		
		//Loop fSize number of times
		for(int i = 0; i < fSize; i++)
		{
			//Read each byte and store each character in variable ch
			fread(&chByte, sizeof(char), 1, cfPtr);
			
			//Combined ch and mask in result by using & bitwise operator
			result = chByte & mask; 
			
			//shift result 6 bits to the right
			result >>= 6;
			
			//shift ch 2 bits to the left
			chByte <<= 2;
			
			//combine ch and result by using OR bitwise operator
			chByte = chByte | result;
		
			//Use complement to change bit in ch
			chByte = ~chByte;
			
			//write the character to the file
			fprintf(encrypted,"%c", chByte);
		}
		//close
		fclose(encrypted);
	}
	//close files 
	fclose(cfPtr);
}