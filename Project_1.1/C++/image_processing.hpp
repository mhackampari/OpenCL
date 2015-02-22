#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

typedef struct {
  int rows;
  int cols;
  int depth;
  unsigned char* header;
  unsigned char* data;
} sImage;

void initImage(sImage* image, int rows, int cols, int depth, unsigned char* headar);
void deleteImage(sImage* image);
void readImage(const char* filename, sImage* image);
void writeImage(const char* filename, sImage* image);

void rgb2grey (sImage* originalImage, sImage* greyImage);
void sobel_edge_detection (sImage* greyImage, sImage* edgeImage);
void grey2rgb (sImage* originalImage, sImage* edgeImage);
void template_matching (sImage* image, sImage* templImage);
void image_twisting (sImage* image, sImage* outputImage, float factor);
void gaussian_filter (sImage* originalImage, sImage* finalImage, float sigma);
void gaussian_grey_filter(sImage* originalImage, sImage* finalImage, float sigma);
void gaussian_separable_filter(sImage* originalImage, sImage* tempImage, sImage* finalImage, float sigma);
void erosion_filter(sImage* originalImage, sImage* outputImage);
void image_downsample(sImage* input, sImage* output, float factor_x, float factor_y);
void motion_detection(sImage* originalImage, sImage* backgroundImage, sImage* outputImage);

#endif
