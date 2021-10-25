/*
Tarpan Patel
Project 3 - 3
CIS 205 -T9
5/8/17
This file was provided by the Professor. I modified the file so it takes arguments from command line.
The program creates an file based on the name giving. The goal was to extract the characters stored in an .bmp
file but its working.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

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
  DWORD x_pixels_meter; //number of pixels per meter in x axis
  DWORD y_pixels_meter; //number of pixels per meter in y axis
  DWORD colors_used; //number of colors used in the bitmap
  DWORD important_colors; //number of important colors

};
#pragma pack(pop)


//-------PROTOTYPES----
BITMAP* read_bitmap(char* file);

void write_bitmap(char* file, BITMAP* bitmap);

//----END PROTOTYPES---

int main(int argc, char** argv)
{
	
	FILE* filePtr; //initialize file pointer
	BITMAP* bmp = NULL; //initialize a struct pointer
	char* in_file_name = argv[1]; //pointer to the argument 1 (bmp image)
	unsigned int mask = 3; //declare a mask of 3
	unsigned char temp; 
	unsigned char char_hold;
	
	//Call read_bitmap function and store pointer to
	bmp=read_bitmap(in_file_name);

	if(bmp == NULL)
	{
		puts("ERROR: Loading File");
	}
	if((filePtr = fopen(argv[2],"w")) == NULL) //Assign filePtr to the file, and write to given file names
	{
		puts("not able to create a file");
	}
	else
	{
		//assign a one byte pointer to the data of bmp image
 		BYTE* bmpByte = (bmp->data);
				
		//colors are in BGR order instead of RGB
		if(bmp->info->bitcount == 24) //24 bit means 8 for each RGB, 32 bit has RGBA
		{
			//loop through the image 
			for(int i = 0; i < bmp->info->img_size; i++)
			{
				//Loop 4 times to get all the octets
				for(int j = 0; j < 4; j++)
				{
					temp = *bmpByte & mask; 	//Mask bmpByte and store it in temp 
					char_hold |= temp; //take temp and use bitwise OR and store the result in char_hold
					char_hold <<=2; //shift char_hold to 2 bits to the left
					bmpByte++; //increment bmpByte pointer
				}
				
				fprintf(filePtr,"%c", char_hold); //print the character to the file
			}
		//close file
		fclose(filePtr);
		}
		//write_bitmap(out_file_name, char_array);
	}
  return 0;
}

//-----FUNCTIONS----

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

  in_file = fopen(file,"r");

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
**
void write_bitmap(char* file, char* char_array)
{

  FILE *out_file;
  //if file does not exist, create it
  out_file = fopen(file,"wb+");
  
	fprintf(out_file, "&s", char_array);
  
  //close the file
  fclose(out_file);
}
*/