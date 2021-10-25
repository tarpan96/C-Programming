/*
Tarpan Patel 
5/6/2017

This file was written by Professor. Only edited the files to get the required output.
This program takes 2 BMP files as arguments from the command line. 
This code was modified so it would allow to take in second bmp file. This program 
takes each green byte from the first bmp image and changes it to RGB byte from the second image.
I was able to change the background of the first image but the image was still green 
and the every R and B byte were faded. I couldn't figure out why it was causing it.

This program reads a bitmap file and writes a bitmap image
The bitmap structure represents the file data. This structure is comprised
of a file header, a bitmap info header, a color table and padding,
and lastly image data.
the read_bitmap function will load a bitmap into these
structures and return a bitmap structure.
once a bitmap structure is constructed it can be write to a file using the
write_bitmap function
In this example, a file is loaded, some file information is printed,
and a new file is written from the structures

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
  DWORD x_pixels_meter; //number of pixels per meter in x axis
  DWORD y_pixels_meter; //number of pixels per meter in y axis
  DWORD colors_used; //number of colors used in the bitmap
  DWORD important_colors; //number of important colors

};
#pragma pack(pop)
//-------END STRUCTURES-----

//-------PROTOTYPES----

BITMAP* read_bitmap(char* file);

void write_bitmap(char* file, BITMAP* bitmap);

float color_dif(BYTE r1, BYTE g1, BYTE b1, BYTE r2, BYTE g2,BYTE b2);

//----END PROTOTYPES---


int main(int argc, char** argv)
{

  BITMAP* bmp = NULL;
  BITMAP* secondbmp = NULL;
  
  //Create pointers to the name
  char* in_file_name = argv[1];
  //take the second argument as image name
  char* second_file = argv[2];
  char* out_file_name = "out.bmp";

  //Call in each 
  bmp=read_bitmap(in_file_name);
  secondbmp=read_bitmap(second_file);

  if(bmp == NULL)
  {
    puts("ERROR: Loading File");
  }
  if(secondbmp == NULL)
  {
	puts("ERROR: Loading Second File");
  }
  else
  {
    //things look ok
    //Do stuff
    //PRINT some bitmap info
    puts("--------------");
    puts("BITMAP INFO");
    puts("--------------");
    printf("BitCount:\t%d\n",bmp->info->bitcount);
    printf("Image Size:\t%.2fMB\n",bmp->info->img_size/1000000.0);//B to MB
    printf("Image Width:\t%d\n",bmp->info->width);
    printf("Image Height:\t%d\n",bmp->info->height);
    printf("Image offset:\t%d\n",bmp->header->offset);
    puts("--------------");
	
	
    //colors are in BGR order instead of RGB
    if(bmp->info->bitcount == 24) //24 bit means 8 for each RGB, 32 bit has RGBA
    {
	    //get the RGB
        for(int i = 0; i < bmp->info->img_size; i+=3)
		{
			//get the RGB
			BYTE* b =  (bmp->data + i);
			BYTE* g =  (bmp->data + i+1);
			BYTE* r =  (bmp->data + i + 2);
			
			//get the RGB for second bmp image 
			BYTE* blue =  (secondbmp->data + i);
			BYTE* green =  (secondbmp->data + i+2);
			BYTE* red =  (secondbmp->data + i + 3);
			
		//change every bmp green byte to every other secondbyte  
		 *g = *(secondbmp->data + i + 1);
		}
	}
    //write the bmp to a file
    write_bitmap(out_file_name, bmp);
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
***/
void write_bitmap(char* file, BITMAP* bmp)
{

  FILE *out_file;
  //if file does not exist, create it
  out_file = fopen(file,"w+");

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
