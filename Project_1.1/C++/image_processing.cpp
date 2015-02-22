#include "image_processing.hpp"
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*
 * Initializes the memory object
 */
void initImage(sImage* image, int rows, int cols, int depth, unsigned char* header){
  image->rows = rows; 
  image->cols = cols; 
  image->depth = depth;   
  image->header = (unsigned char *)malloc(sizeof(unsigned char)*54L);
  image->data = (unsigned char *)malloc(sizeof(unsigned char)*rows*cols*depth);
  if(header)
    memcpy(image->header,header,54L);
}

/*
 * Deletes the memory object
 */
void deleteImage(sImage* image){
  if(image->header)
    free(image->header);
  if(image->data)
    free(image->data);
  image->rows = image->cols = image->depth = 0;
  image->header = image->data = NULL;
}

/*
 * Reads a 24bit RGB image from a file and saves it in a memory object (required memory is instantiated here)
 */
void readImage(const char* filename, sImage* image){
  FILE *bmpInput;
  int bits = 0;
  int fileSize = 0;
  int results;
  
  
  int x = fopen_s(&bmpInput, filename, "rb");
  
  if(bmpInput){
  
	  
    image->header = (unsigned char *)malloc(sizeof(unsigned char) * 54L);
    results=fread(image->header, sizeof(char)*54L, 1, bmpInput);
    
    memcpy(&fileSize,&image->header[2],4);
    memcpy(&image->cols,&image->header[18],4);
    memcpy(&image->rows,&image->header[22],4);
    memcpy(&bits,&image->header[28],4);
    image->depth=3;

    printf("Width: %d\n", image->cols);
    printf("Height: %d\n", image->rows);
    printf("File size: %ld\n", fileSize);
    printf("Bits/pixel: %d\n", bits);
    
    if(bits!=24 || fileSize!=image->rows*image->cols*image->depth+54){
      printf("Wrong image format in %s: accepted only 24 bit without padding!\n", filename);
      exit(0);
    }
      
    image->data = (unsigned char *)malloc(image->rows*image->cols*sizeof(unsigned char)*3);
    fseek(bmpInput, 54L, SEEK_SET);
    results=fread(image->data, sizeof(char), (image->rows*image->cols*image->depth), bmpInput);
    //printf("File read\n");

    fclose(bmpInput);
  } else {
    printf("File not found: %s\n",filename);
    exit(0);  
  }
}

/*
 * Saves an image in a file
 */
void writeImage(const char* filename, sImage* image){
  FILE *bmpOutput;
  fopen_s(&bmpOutput,filename, "wb");  
  if(bmpOutput){
    fwrite(image->header, sizeof(char), 54L, bmpOutput);
    fwrite(image->data, sizeof(char), (image->rows*image->cols*image->depth), bmpOutput);
    fclose(bmpOutput);
  } else {
    printf("File not opened: %s\n",filename);
    exit(0);  
  }
}

/*
 * Converts a grey-scale image in RGB format and saves it in another object. Memory has to be already instantiated
 */
void grey2rgb(sImage* greyImage, sImage* outputImage){
  int i=0;
  for(i = 0; i < (outputImage->rows*outputImage->cols); i++){
    *(outputImage->data + i*3) = *(greyImage->data +i);
    *(outputImage->data + i*3 +1) = *(greyImage->data +i);
    *(outputImage->data + i*3 +2) = *(greyImage->data +i);
  }
}

/*
 * Converts an RGB image in grey-scale format and saves it in another object. Memory has to be already instantiated
 */
void rgb2grey (sImage* originalImage, sImage* greyImage){
  //TODO clean this code... it has been cut and pasted from another project...
  int r = 0;
  int c = 0;
  int  redValue, greenValue, blueValue, grayValue;
  unsigned char someChar;
  unsigned char *pChar;
  
  someChar = '0';
  pChar = &someChar;
    
  for(r=0; r<originalImage->rows; r++){
    for (c=0; c<originalImage->cols; c++){
      /*-----READ FIRST BYTE TO GET BLUE VALUE-----*/
      *pChar = *(originalImage->data + (r*originalImage->cols + c)*3);
      blueValue = *pChar;
            
      /*-----READ NEXT BYTE TO GET GREEN VALUE-----*/
      *pChar = *(originalImage->data + (r*originalImage->cols +c)*3+1);
      greenValue = *pChar;
            
      /*-----READ NEXT BYTE TO GET RED VALUE-----*/
      *pChar = *(originalImage->data + (r*originalImage->cols +c)*3+2);
      redValue = *pChar;
            
      /*-----USE FORMULA TO CONVERT RGB VALUE TO GRAYSCALE-----*/
      grayValue = (int)(0.299*redValue + 0.587*greenValue + 0.114*blueValue);
                       
      /*-----WRITE TO NEW BMP FILE------*/
      *pChar = grayValue;
      *(greyImage->data + r*originalImage->cols +c) = *pChar;
      }
  }    
}

