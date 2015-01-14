#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_
#include <string>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  int rows;
  int cols;
  unsigned char* header;
  unsigned char* data;
} sImage;


void readImage(std::string filename, sImage* image);
void deleteImage(sImage* image);
void writeImage(std::string filename, sImage* image);
void rgb2grey (sImage* originalImage, sImage* greyImage);
void sobel_edge_detection (sImage* greyImage, sImage* edgeImage);
void grey2rgb (sImage* originalImage, sImage* edgeImage);
void template_matching (sImage* image, sImage* templImage);
void image_twist (sImage* image, sImage* templImage);
int padding(int image_size, float scale_factor);

#endif
