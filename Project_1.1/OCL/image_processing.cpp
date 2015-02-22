#include "image_processing.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
float tempo;
void readImage(std::string filename, sImage* image){
	FILE *bmpInput;
	int bits;
	long fileSize;

	printf("Reading file %s\n", filename);
	char* temp =(char*)alloca(filename.size()+1);
	memcpy(temp, filename.c_str(), filename.size() + 1);
	bmpInput = fopen(temp, "rb");
	if (bmpInput){

		image->header = (unsigned char *)malloc(sizeof(unsigned char)* 54L);
		fread(image->header, sizeof(char)* 54L, 1, bmpInput);

		memcpy(&fileSize, &image->header[2], 4);
		memcpy(&image->cols, &image->header[18], 4);
		memcpy(&image->rows, &image->header[22], 4);
		memcpy(&bits, &image->header[28], 4);

		printf("Width: %d\n", image->cols);
		printf("Height: %d\n", image->rows);
		printf("File size: %ld\n", fileSize);
		printf("Bits/pixel: %d\n", bits);

		if (bits != 24 || fileSize != image->rows*image->cols * 3 + 54){
			printf("Wrong image format: accepted only 24 bit without padding!\n");
			exit(0);
		}

		image->data = (unsigned char *)malloc(image->rows*image->cols*sizeof(unsigned char)* 3);
		fseek(bmpInput, 54L, SEEK_SET);
		fread(image->data, sizeof(char), (image->rows*image->cols * 3), bmpInput);
		printf("File read\n");

		fclose(bmpInput);
	}
	else {
		printf("File not found\n");
		exit(0);
	}
}

void deleteImage(sImage* image){
	if (image->header)
		free(image->header);
	if (image->data)
		free(image->data);
	image->rows = image->cols = 0;
	image->header = image->data = NULL;
}

void writeImage(std::string filename, sImage* image){
	FILE *bmpOutput;
	char* temp = (char *)filename.c_str();
	bmpOutput = fopen(temp, "wb");
	if (bmpOutput){
		fwrite(image->header, sizeof(char), 54L, bmpOutput);
		fwrite(image->data, sizeof(char), (image->rows*image->cols * 3), bmpOutput);
		fclose(bmpOutput);
	}
	else {
		printf("File not opened\n");
		exit(0);
	}
}

void grey2rgb(sImage* originalImage, sImage* edgeImage){
	int i = 0;
	for (i = 0; i < (originalImage->rows*originalImage->cols); i++){
		*(originalImage->data + i * 3) = *(edgeImage->data + i);
		*(originalImage->data + i * 3 + 1) = *(edgeImage->data + i);
		*(originalImage->data + i * 3 + 2) = *(edgeImage->data + i);
	}
}

void rgb2grey(sImage* originalImage, sImage* greyImage){
	int r = 0;
	int c = 0;
	int  redValue, greenValue, blueValue, grayValue;
	unsigned char someChar;
	unsigned char *pChar;

	someChar = '0';
	pChar = &someChar;

	for (r = 0; r<originalImage->rows; r++){
		for (c = 0; c<originalImage->cols; c++){
			/*-----READ FIRST BYTE TO GET BLUE VALUE-----*/
			*pChar = *(originalImage->data + (r*originalImage->cols + c) * 3);
			blueValue = *pChar;

			/*-----READ NEXT BYTE TO GET GREEN VALUE-----*/
			*pChar = *(originalImage->data + (r*originalImage->cols + c) * 3 + 1);
			greenValue = *pChar;

			/*-----READ NEXT BYTE TO GET RED VALUE-----*/
			*pChar = *(originalImage->data + (r*originalImage->cols + c) * 3 + 2);
			redValue = *pChar;

			/*-----USE FORMULA TO CONVERT RGB VALUE TO GRAYSCALE-----*/
			grayValue = (int)(0.299*redValue + 0.587*greenValue + 0.114*blueValue);


			/*-----WRITE TO NEW BMP FILE------*/
			*pChar = grayValue;
			*(greyImage->data + r*originalImage->cols + c) = *pChar;
		}
	}

}

int padding(int image_size, float scale_factor){

	int temp = floor(image_size / scale_factor);
	int rest = temp * 3 % 4;
	return temp + rest;
}
