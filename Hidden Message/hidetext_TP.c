/*
Tarpan Patel
Project 3 - 3
CIS 205 -T9
This program is created by Professor Penta. I modified the file so it arguments from command line. 
This program text file stores 2 bits from an byte that is from the text file into the image bytes. 
The goal of the program was to hide characters into given bmp image. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h> //for datatypes wit specific widths (uint8_t, uint16_t...)
//---------TYPEDEF-----------

typedef struct header HEADER; //bitmap file header
typedef struct info INFO; //bitmap image info header

typedef struct bitmap BITMAP;
typedef uint8_t BYTE; //one byte
typedef uint16_t WORD; //two bytes
typedef uint32_t DWORD; //four bytes

//-------END TYPEDEF---------

//--------STRUCTURES--------

struct bitmap
{
  HEADER* header; //bitmap file header
  INFO* info; //bitmap image info header
  BYTE* extra; //color table, padding, etc
  BYTE* data; //image data
};

#pragma pack(push, 1)
struct header //14 bytes - info aabout the file
{
  WORD signature; //BM for bitmap
  DWORD file_size; //Size of the entire BMP file
  WORD reserved1;  //reserved; must be 0
  WORD reserved2;  //reserved; must be 0
  DWORD offset; //the location of the image data
};
#pragma pack(pop)

#pragma pack(push, 1)
struct info //40 bytes - info about the image
{
  DWORD struct_size; //size of this struct - 40 bytes
  DWORD width;//width of the image
  DWORD height;//height of the image
  WORD planes;//layers in image
  WORD bitcount;//number of bits per pixel
  DWORD compression;///spcifies the type of compression
  DWORD img_size;  //size of image in bytes
};
#pragma pack(pop)
//-------END STRUCTURES-----

//-------PROTOTYPES----

BITMAP* read_bitmap(char* file);

void write_bitmap(char* file, BITMAP* bitmap);

void hideText(BYTE*, char*, int);
float color_dif(BYTE r1, BYTE g1, BYTE b1, BYTE r2, BYTE g2,BYTE b2);

//----END PROTOTYPES---

int main(int argc, char** argv)
{
	FILE* filePtr;  //initialize file pointer
	BITMAP* bmp = NULL; //initialize a struct pointer
	char* in_file_name = argv[1]; //Initialize a char pointer and assign argument 2
	char* out_file_name = "out.bmp"; //Name the outcome
	char* char_array; //initialize a character array
	unsigned int mask = 3; //define a mask and assign value of 3
	unsigned char temp; //hold temp char variable
  
    //assign bmp to 
	bmp=read_bitmap(in_file_name);

	if(bmp == NULL) //check if bmp is empty
	{
		puts("ERROR: Loading File");
	}

	
	if((filePtr = fopen(argv[2], "r")) == NULL) //Assign filePtr to text file
	{
		puts("No File found");
	}
	else
	{
		//assign bmpByte to data of bmp image
		BYTE* bmpByte = (bmp->data);

		fseek(filePtr, 0, SEEK_END); //Set the pointer to end of the file
		int fileSize = ftell(filePtr); //Grab the size of the file ans assign it to fileSize
		fseek(filePtr, 0, SEEK_SET); //Set filePtr to the beginning
		
	    char_array = malloc(fileSize); //malloc memory based on filePtr
	 
	    //Loop through the number of fileSize
		for(int i = 0; i < fileSize; i++)
		{
			//Read in each byte from the file
			fread(&char_array[i], sizeof(BYTE), 1, filePtr);
		}	
		
		
		hideText(bmpByte, char_array, fileSize); //Call hideText function to store character into bmp
		
		//Call write_bitmap to close bmp file
		write_bitmap(out_file_name, bmp);
		
		//close file
		fclose(filePtr);
		
	}
  return 0;
}

//-----FUNCTIONS----


void hideText(BYTE* bmpByte, char* char_array, int fileSize)
{
	
	unsigned int mask = 3; //initialize mask of 3
	unsigned char temp;	//initialize variable temp
	
		//Loop fileSize times
		for(int i = 0; i <= fileSize; i++)
		{
			for(int j = 0; j < 4; j++) //Loop through each byte 4 time 
			{
				temp = char_array[i] & mask; //combine character from array to mask and store to temp
				*bmpByte &= (~mask); //take the complement of mask and combine to bmpByte using & operator
				*bmpByte |= temp; //Combine temp to bmpByte
				bmpByte++; //increment bmpByte
				char_array[i] >>= 2; //shift the character byte to the right 2 bits
			}
		}
}

/**
* loads a bitmap file into memory
* arguments: a c string representing a path to the file
* return value: a pointer to a BITMAP struct
***/
BITMAP* read_bitmap(char* file)
{
  BITMAP* bmp = NULL;
  FILE *in_file = NULL;
  

  int error = 0;

  in_file = fopen(file,"rb+");

  if(in_file==NULL)
  {
    puts("ERROR: no input file found");
    error = 1;
  }
  else
  {
      //file exists
      //allocate bitmap struct
      bmp =malloc(sizeof(BITMAP));

      //allocate the header struct
      bmp->header = malloc(sizeof(HEADER));
      //read the header into the struct
      fread(bmp->header, sizeof(HEADER), 1, in_file);

      if(bmp->header->signature != 0x4D42)
      {
        puts("ERROR: not bmp file format");
        error=1;
      }
  }

  //File loaded and data is bitmap
  if(error == 0)
  {
    //allocate and read the information header
    bmp->info = malloc(sizeof(INFO));
    fread(bmp->info, sizeof(INFO), 1, in_file);

    //collect everything between the header/infoheader and the img data
    //color table, padding, etc
    size_t size  = bmp->header->offset - (sizeof(HEADER) + sizeof(INFO));
    bmp->extra = malloc(size);
    fread(bmp->extra, size, 1, in_file);

    //allocate and read the actual image data
    bmp->data = malloc(bmp->info->img_size);
    fread(bmp->data, bmp->info->img_size, 1, in_file);
  }
  else
  {
    //something went wrong
    //clean up what has been allocated
    //and set bmp (return value) to NULL
    free(bmp->header);
    free(bmp);
    bmp = NULL;
  }

  //close the file and return bitmap or NULL
  fclose(in_file);
  return bmp;
}


/*
* write a bitmap file into memory
* arguments: a c string representing a path to the file and a bitmap struct
*      a file is created if it does not exist.
* return value: nothing
***/
void write_bitmap(char* file, BITMAP* bmp)
{

  FILE *out_file;
  //if file does not exist, create it
  out_file = fopen(file,"wb+");

  //figure out how big the extra struct is
  size_t size  = bmp->header->offset - (sizeof(HEADER) +sizeof(INFO));

  //write all the parts
  fwrite(bmp->header, sizeof(HEADER), 1, out_file);
  fwrite(bmp->info, sizeof(INFO), 1, out_file);
  fwrite(bmp->extra, size, 1, out_file); //math above for this
  fwrite(bmp->data, bmp->info->img_size, 1, out_file);

  //close the file
  fclose(out_file);
}
