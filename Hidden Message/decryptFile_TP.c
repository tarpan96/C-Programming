/*
Tarpan Patel 
Project3 - 1b
CIS 205 -T9
5/4/2017
This program takes in an encrypted file as an argument 
from command line and decrypts the file using ditwise operators.
*/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

int main(int argc, char** argv)
{
	//Initialize pointers for files
	FILE* cfPtr;
	FILE* decryptedFile;
	
	//Open file and read file
	if((cfPtr = fopen(argv[1], "rb+")) == NULL)
	{
		puts("No file found");
	}
	else
	{
		//initialize variables 
		int fSize;
		BYTE chByte;
		BYTE mask = 3;
		BYTE result;
		
		//Find the size of the file by moving pointer to the EOF
		fseek(cfPtr, 0, SEEK_END);
		
		//assign the file size to fSize
		fSize = ftell(cfPtr);
		
		//Set pointer back to the beginning 
		fseek(cfPtr, 0, SEEK_SET);
		
		//create fileDecrypted.txt file and write in it
		decryptedFile = (fopen("fileDecrypted.txt", "wb+"));
		
		//Loop until end of the file size
		for(int i = 0; i < fSize; i++)
		{
			//Read each byte and store it in chByte
			fread(&chByte, sizeof(char), 1, cfPtr);
			
			//Use bitwise ~ to get opposite bits of chByte
			chByte = ~chByte;
			
			//Combined ch and mask in result by using & bitwise operator
			result = chByte & mask; 
			
			//shift result 6 bits to the left
			result <<= 6;
			
			//shift chByte 2 bits to the right
			chByte >>= 2;
			
			//combine ch and result by using OR bitwise operator
			chByte = chByte | result;
			
			//write each byte to the file
			fprintf(decryptedFile,"%c",chByte);
		}
		//close file
		fclose(decryptedFile);
	}
	//close file
	fclose(cfPtr);
}