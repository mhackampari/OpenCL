#include "image_processing.hpp"
#include "stdafx.h"

#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

//image twisting macros
#define PI 3.1415265359
#define DRAD 180.0f/PI

//motion detection macros
#define MD_THRESHOLD 10
#define EROSION_MAP_SIZE 7 /*must be an odd number*/
#define FG 0
#define BG 255
#define ER_THRESHOLD 0.8
#define M_THRESHOLD 10

/*
 * Performs the edge detection according to the Sobel algorithm
 */
void sobel_edge_detection(sImage* greyImage, sImage* edgeImage){
  int  GX[3][3];
  int  GY[3][3];
    
  unsigned int X, Y;
  int  I, J;
  long sumX, sumY;
  int  SUM;
    
  /* 3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
  GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
  GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
  GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;
    
  /* 3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
  GY[0][0] =  1; GY[0][1] =  2; GY[0][2] =  1;
  GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
  GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;
    
  /*---------------------------------------------------
    SOBEL ALGORITHM STARTS HERE
    ---------------------------------------------------*/
  #pragma omp parallel for private(sumX, sumY, SUM, I, J, X, Y) //default (shared) // shared(edgeImage, GX, GY, greyImage) //collapse(2)
  for(int Y=0; Y<=(greyImage->rows-1); Y++)  {
    for(int X=0; X<=(greyImage->cols-1); X++)  {
      sumX = 0;
      sumY = 0;
            
      /* image boundaries */
      if(Y==0 || Y==greyImage->rows-1 || X==0 || X==greyImage->cols-1)
        SUM = 0;
      /* Convolution starts here */
      else   {              
        /*-------X GRADIENT APPROXIMATION------*/
        for(I=-1; I<=1; I++)  {
          for(J=-1; J<=1; J++)  {
            sumX = sumX + (int)( (*(greyImage->data + X + I + (Y + J)*greyImage->cols)) * GX[I+1][J+1]);
          }
        }
        //if(sumX>255) sumX=255;
        //else if(sumX<0) sumX=0;
             
        /*-------Y GRADIENT APPROXIMATION-------*/
        for(I=-1; I<=1; I++)  {
          for(J=-1; J<=1; J++)  {
            sumY = sumY + (int)( (*(greyImage->data + X + I + (Y + J)*greyImage->cols)) * GY[I+1][J+1]);
          }
        }
        //if(sumY>255) sumY=255;
        //else if(sumY<0) sumY=0;
                
        //SUM = abs(sumX) + abs(sumY); /*---GRADIENT MAGNITUDE APPROXIMATION (Myler p.218)----*/
        SUM= sqrt(double(sumX*sumX+sumY*sumY));
      }        
      *(edgeImage->data + X + Y*greyImage->cols) = /*255 -*/ (unsigned char)(SUM);
    }
  }
}

/*
 * Performs the image downsampling by using the bilinear interpolation
 */
void image_downsample(sImage* image, sImage* outputImage, float factor_x, float factor_y){
  //http://en.wikipedia.org/wiki/Bilinear_interpolation
  int r, c; //current coordinates
  int olc, ohc, olr, ohr; //coordinates of the original image used for bilinear interpolation
  int index; //linearized index of the point  
  unsigned char q11, q12, q21, q22;
  float accurate_c, accurate_r; //the exact scaled point
  int k;


  
  
  #pragma omp parallel for private(r, c, olc, ohc, olr, ohr, index, q11, q12, q21, q22, k, accurate_c, accurate_r) //default(shared) //shared(edgeImage, GX, GY, greyImage) //collapse(2)
  for(r=0; r<outputImage->rows; r++){
    for(c=0; c<outputImage->cols; c++){
      accurate_c = (float)c*factor_x;
      olc=accurate_c;
      ohc=olc+1;
      if(!(ohc<image->cols))
        ohc=olc;

      accurate_r = (float)r*factor_y;
      olr=accurate_r;
      ohr=olr+1;
      if(!(ohr<image->cols))
        ohr=olr;
             
      index= (c + r*outputImage->cols)*outputImage->depth; //outputImage->depth bytes per pixel
      for(k=0; k<outputImage->depth; k++){
        q11=image->data[(olc + olr*image->cols)*outputImage->depth+k];
        q12=image->data[(olc + ohr*image->cols)*outputImage->depth+k];
        q21=image->data[(ohc + olr*image->cols)*outputImage->depth+k];
        q22=image->data[(ohc + ohr*image->cols)*outputImage->depth+k];
        outputImage->data[index+k] = (unsigned char) (q11*(ohc-accurate_c)*(ohr-accurate_r) + 
                                                     q21*(accurate_c-olc)*(ohr-accurate_r) + 
                                                     q12*(ohc-accurate_c)*(accurate_r-olr) +
                                                     q22*(accurate_c-olc)*(accurate_r-olr));
      }
    }  
  }
}

/*
 * Performs an image twisting
 */ 
void image_twisting(sImage* image, sImage* outputImage, float factor){
  int r, c; //current coordinates
  int cr, cc; //coordinates of the center of the image
  float x, y, radius, theta; //cartesian coordinates and polar ones of the twisted point (x=column number, y=row number)
  int index; //linearized index of the point
  float distortion_gain = 1000.0 * factor;
  
  //http://en.wikipedia.org/wiki/Bilinear_interpolation
  int lx, ly, hx, hy; //4 coordinates for bilinear interpolation
  unsigned char q11, q12, q21, q22;
  int k;
     
  cr=image->rows/2;
  cc=image->cols/2;

  #pragma omp parallel for private (r, c, theta, radius, x, y, index, lx, hx, ly, hy, q11, q12, q21, q22, k) //default (shared)
  for(r=0; r<image->rows; r++){
    for(c=0; c<image->cols; c++){
      radius = sqrt(double((c - cc) * (c - cc) + (r - cr) * (r - cr)));
      theta = radius/DRAD/image->rows*distortion_gain; //radius/2*DRAD;
      
      x = cos(theta) * (c - cc) + sin(theta) * (r - cr) + cc;
      y = -sin(theta) * (c - cc) + cos(theta) * (r - cr) + cr;
      index= (c + r*image->cols)*outputImage->depth; //outputImage->depth bytes per pixel
      if(x>=0 && y>=0 && y<image->rows-1 && x<image->cols-1) {
        //bilinear interpolation
        lx=x;
        hx=x+1;
        ly=y;
        hy=y+1;
        
        for(k=0; k<outputImage->depth; k++){
          q11=image->data[(lx + ly*image->cols)*outputImage->depth+k];
          q12=image->data[(lx + hy*image->cols)*outputImage->depth+k];
          q21=image->data[(hx + ly*image->cols)*outputImage->depth+k];
          q22=image->data[(hx + hy*image->cols)*outputImage->depth+k];
          outputImage->data[index+k]= (unsigned char) (q11*(hx-x)*(hy-y) + 
                                                     q21*(x-lx)*(hy-y) + 
                                                     q12*(hx-x)*(y-ly) +
                                                     q22*(x-lx)*(y-ly));
        }         
      } else{
        for(k=0; k<outputImage->depth; k++){
          outputImage->data[index+k] = 0;
        }      
      }
    }  
  }
}

/*
 * Performs the motion detection
 */
void motion_detection(sImage* originalImage, sImage* backgroundImage, sImage* outputImage){
  int r,c, temp;
  #pragma omp parallel for private(r, c) //default(shared) //shared(edgeImage, GX, GY, greyImage) //collapse(2)
  for(r=0; r<originalImage->rows; r++){
    for (c=0; c<originalImage->cols; c++){
      temp=abs(originalImage->data[r*originalImage->cols+c]-backgroundImage->data[r*originalImage->cols+c])> M_THRESHOLD ? FG : BG;
	  outputImage->data[r*originalImage->cols + c] = temp;
    }
  }  
 
}

/*
 * Performs the erosion filter on the motion detection image
 */
void erosion_filter(sImage* originalImage, sImage* outputImage){
  int r,c, r1, c1, marker, count;
  #pragma omp parallel for private(r, c, r1, c1, marker, count) //default(shared) //shared(edgeImage, GX, GY, greyImage) //collapse(2)
  for(r=0; r<originalImage->rows; r++){
    for (c=0; c<originalImage->cols; c++){
      marker=0;
      count=0;
      for(r1=(r-(EROSION_MAP_SIZE-1)/2>0)?(r-(EROSION_MAP_SIZE-1)/2):0; r1<r+(EROSION_MAP_SIZE-1)/2 && r1<originalImage->rows; r1++){
        for(c1=(c-(EROSION_MAP_SIZE-1)/2>0)?c-(EROSION_MAP_SIZE-1)/2:0; c1<c+(EROSION_MAP_SIZE-1)/2 && c1<originalImage->cols; c1++){
          count++;
          marker+=(originalImage->data[r1*originalImage->cols+c1]==FG)? 1:0;
        }
      }
      if((float)marker/count>ER_THRESHOLD){
        outputImage->data[r*originalImage->cols+c]=FG;
      } else {
        outputImage->data[r*originalImage->cols+c]=BG;                
      }
    }
  }  
}

/*
 * Performs the gaussian filter
 */
void gaussian_filter(sImage* originalImage, sImage* finalImage, float sigma){    
  int r, c, r1, c1;
  int  I, J, color;
  long sumX, sumY;
  float  SUM;

  //generate the filter mask
  int maskSize = (int)ceil(3.0f*sigma);
  float * mask = new float[(maskSize * 2 + 1)*(maskSize * 2 + 1)];
  float sum = 0.0f;
  for (int a = -maskSize; a < maskSize + 1; a++) {
    for (int b = -maskSize; b < maskSize + 1; b++) {
      float temp = exp(-((float)(a*a + b*b) / (2 * sigma*sigma)));
        sum += temp;
        mask[a + maskSize + (b + maskSize)*(maskSize * 2 + 1)] = temp;
      }
  }
  // Normalize the mask
  for (int i = 0; i < (maskSize * 2 + 1)*(maskSize * 2 + 1); i++)
	  mask[i] = mask[i] / sum;

#pragma omp parallel for private(r, c, color, sum, I, J) 
  for (r = 0; r <= (originalImage->rows - 1); r++)  {
	  for (c = 0; c <= (originalImage->cols - 1); c++)  {
		  for (color = 0; color < originalImage->depth; color++) {
			  sum = 0;
			  /* image boundaries */
			  if (c <= maskSize || c >= originalImage->cols - maskSize || r <= maskSize || r >= originalImage->rows - maskSize)
				  sum = (float)originalImage->data[(c + r*originalImage->cols)*originalImage->depth + color];
			  else {/* Convolution starts here */
				  for (I = -maskSize; I <= maskSize; I++)  {
					  for (J = -maskSize; J <= maskSize; J++)  {
						  sum = sum + (float)originalImage->data[((c + I) + (r + J)*originalImage->cols)*originalImage->depth + color]
							  * mask[I + maskSize + (J + maskSize)*(maskSize * 2 + 1)];
					  }
				  }
			  }
			  finalImage->data[(c + r*originalImage->cols)*originalImage->depth + color] = (unsigned char)(sum);
		  }
	  }
  }
  
  
  delete [] mask;
}



void gaussian_grey_filter(sImage* originalImage, sImage* finalImage, float sigma){
	int r, c, r1, c1, w, h;
	int  I, J, color;
	long sumX, sumY;
	float  SUM;

	//generate the filter mask
	int maskSize = (int)ceil(3.0f*sigma);
	float * mask = new float[(maskSize * 2 + 1)*(maskSize * 2 + 1)];
	float sum = 0.0f;
	for (int a = -maskSize; a < maskSize + 1; a++) {
		for (int b = -maskSize; b < maskSize + 1; b++) {
			float temp = exp(-((float)(a*a + b*b) / (2 * sigma*sigma)));
			sum += temp;
			mask[a + maskSize + (b + maskSize)*(maskSize * 2 + 1)] = temp;
		}
	}
	// Normalize the mask
	for (int i = 0; i < (maskSize * 2 + 1)*(maskSize * 2 + 1); i++)
		mask[i] = mask[i] / sum;

#pragma omp parallel for private(r, c, r1, c1, sum) //shared(finalImage, originalImage, mask) //collapse(2)
		for (r = 0; r <= (originalImage->rows - 1); r++)  {
			for (c = 0; c <= (originalImage->cols - 1); c++)  {
				sum = 0;
				/* image boundaries */
				/*if (r - maskSize >= 0) I = r - maskSize;
				else I = 0;

				if (c - maskSize >= 0) J = c - maskSize;
				else J = 0;

				h = 0;

				for (I; I <= r + 2*maskSize + 1 && I < originalImage->rows && h < 2 * maskSize + 1; I++)  {

				w = 0;

				for (J; J <= c + 2*maskSize + 1 && J < originalImage->cols && w < 2 * maskSize + 1; J++)  {
				sum = sum + (float)originalImage->data[J + I*originalImage->cols]
				* mask[w + (2 * maskSize + 1)*h];
				w++;
				}
				h++;
				}*/
				w = c - maskSize >= 0 ? 0 : maskSize - c;
				h = r - maskSize >= 0 ? 0 : maskSize - r;

				for (int r1 = (r - maskSize >= 0) ? r - maskSize : 0; r1 <= r + maskSize && r1 < originalImage->rows && h < 2 * maskSize + 1; r1++){
					w = c - maskSize >= 0 ? 0 : maskSize - c;

					for (int c1 = (c - maskSize >= 0) ? c - maskSize : 0; c1 <= c + maskSize && c1 < originalImage->cols && w < 2 * maskSize + 1; c1++){

						sum += mask[w + (2 * maskSize + 1)*h] * originalImage->data[r1*originalImage->cols + c1];
						w++;
					}

					h++;
				}

				finalImage->data[(c + r*originalImage->cols)] = (unsigned char)(sum);

			}
		}

	delete[] mask;
}



void gaussian_separable_filter(sImage* originalImage, sImage* tempImage, sImage* finalImage, float sigma){
	int r, c;
	int  I, J, color, w;
	long sumX, sumY;
	float  SUM;

	//generate the 1D filter mask
	int maskSize = (int)ceil(3.0f*sigma);
	float* mask = new float[maskSize * 2 + 1];
	float sum = 0.0f;

	for (int b = -maskSize; b < maskSize + 1; b++) {
		float temp = exp(-((float)(b*b) / (2 * sigma*sigma)));
		sum += temp;
		mask[b + maskSize] = temp;
	}
	
	// Normalize the mask
	for (int i = 0; i < maskSize * 2 + 1; i++)
		mask[i] = mask[i] / sum;


#pragma omp parallel for private(r, c, color, sum, I, J) //shared(finalImage, originalImage, mask) //collapse(2)
	for (r = 0; r <= (originalImage->rows - 1); r++)  {
		for (c = 0; c <= (originalImage->cols - 1); c++)  {

			sum = 0;

			for (J = -maskSize; J <= maskSize; J++)  {

				if ((c + J) < 0) w = - J;
				else if ((originalImage->cols - 1) - (c + J ) < 0) w = originalImage->cols - 1 - (J-maskSize);
				else w = c + J;

				sum = sum + (float)originalImage->data[(w + r*originalImage->cols)] * mask[J + maskSize];
						}

				tempImage->data[(c + r*originalImage->cols)] = (unsigned char)(sum);
			
		}
	}

#pragma omp parallel for private(r, c, color, sum, I, J) //shared(finalImage, originalImage, mask) //collapse(2)
	for (r = 0; r <= (originalImage->rows - 1); r++)  {
		for (c = 0; c <= (originalImage->cols - 1); c++)  {

			sum = 0;

			for (J = -maskSize; J <= maskSize; J++){

				if ((r + J) < 0) w = -J;
				else if ((originalImage->cols - 1) - (r + J) < 0) w = originalImage->cols - 1 - (J-maskSize);
				else w = r + J;

				sum = sum + (float)tempImage->data[(c + w*originalImage->cols)] * mask[J + maskSize];
			}

			finalImage->data[(c + r*originalImage->cols)] = (unsigned char)(sum);

		}
	}

	delete[] mask;
}


