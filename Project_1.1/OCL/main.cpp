#include "image_processing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <CL/cl.h>
#include <CL\CLUtil.hpp>
#include "MotionHeader.hpp"
#include "GaussianHeader.h"
#include "ErosionHeader.h"
#include "DSamplerHeader.h"
#include <array>
#include <iomanip>

using namespace std;

#include <math.h>


int main(int argc, char* argv[]){
	sImage motionImage, greyMotionImage, bgImage, greyBGImage, outputImage, greyFinalImage, gaussOutputImage, gaussImage, gaussGreyImage, originalImage, downsampledImage;
	ofstream benchmark; benchmark.open("benchamark_manual.txt", ofstream::app);
	int fileSize;
	float factor, tempo=0;
	string cat[]= { "cat025.bmp" , "cat05.bmp" ,  "cat.bmp" , "2cat.bmp", "4cat.bmp" };
	string back[] = { "background025.bmp", "background05.bmp", "background.bmp", "2background.bmp", "4background.bmp" };
	string motion[] = { "motion025.bmp", "motion05.bmp", "motion.bmp", "2motion.bmp", "4motion.bmp" };
	char *temp;
	char *temp2;
	char *temp3;
	char *temp4;

	for (int i = 0; i < 2; i++){
		if (i == 0) benchmark << "\n*******************************\n*******************************\nCPU measurements are : \n";
		else if (i == 1) benchmark << "\n\n*******************************\n*******************************\nGPU measurements are:\n";
		for (int count = 0; count < 5; count++){
			//Opening function from image_processing.h
			
			readImage(cat[count], &gaussImage);

			readImage(motion[count], &motionImage);

			readImage(back[count], &bgImage);

			readImage(cat[count], &originalImage);


			//Initilizing the variables
			gaussGreyImage.rows = gaussImage.rows;
			gaussGreyImage.cols = gaussImage.cols;
			gaussGreyImage.data = (unsigned char *)malloc(gaussImage.rows*gaussImage.cols*sizeof(unsigned char));
			gaussGreyImage.header = NULL;

			gaussOutputImage.rows = gaussImage.rows;
			gaussOutputImage.cols = gaussImage.cols;
			gaussOutputImage.data = (unsigned char *)malloc(gaussImage.rows*gaussImage.cols*sizeof(unsigned char));
			gaussOutputImage.header = NULL;

			greyBGImage.rows = bgImage.rows;
			greyBGImage.cols = bgImage.cols;
			greyBGImage.data = (unsigned char *)malloc(bgImage.rows*bgImage.cols*sizeof(unsigned char));
			greyBGImage.header = NULL;

			greyMotionImage.rows = bgImage.rows;
			greyMotionImage.cols = bgImage.cols;
			greyMotionImage.data = (unsigned char *)malloc(bgImage.rows*bgImage.cols*sizeof(unsigned char));
			greyMotionImage.header = NULL;

			greyFinalImage.rows = bgImage.rows;
			greyFinalImage.cols = bgImage.cols;
			greyFinalImage.data = (unsigned char *)malloc(bgImage.rows*bgImage.cols*sizeof(unsigned char));
			greyFinalImage.header = NULL;

			outputImage.rows = bgImage.rows;
			outputImage.cols = bgImage.cols;
			outputImage.data = (unsigned char *)malloc(bgImage.rows*bgImage.cols*sizeof(unsigned char));
			outputImage.header = NULL;

			//Transforming rgb images into grey scale 
			rgb2grey(&gaussImage, &gaussGreyImage);
			rgb2grey(&motionImage, &greyMotionImage);
			rgb2grey(&bgImage, &greyBGImage);

			MotionDetector clMotionDetector;
			GaussianBlur gaussianBlur;
			Erosion clErosion;
			DSampling cldownsampler;
			cl_device_type deviceType;

			
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			//DOWNSCALING
			/////////////////////////////////////////////////////////////////////////////////////////////////////
				
			//Choosing the computation device
			/*i = -1;
			while (i != 1 && i != 0){
				cout << "insert 0 - CPU \t 1 - GPU: " << endl;
				cin >> i;
			}*/
			if (i == 0)
				deviceType = CL_DEVICE_TYPE_CPU;
			if (i == 1)
				deviceType = CL_DEVICE_TYPE_GPU;
			int j = 4;
			

				factor = 2;

				//Operations necessary to initialize a new downscaled .bmp file where the final result is wroten back.

				//First we have right scaled dimensions, it's done with padding control
				//http://stackoverflow.com/questions/2601365/padding-in-24-bits-rgb-bitmap

				float factor_y = (float)originalImage.rows / padding(originalImage.rows, factor);
				if (factor_y != factor){
					factor_y += 0.00001;
				}
				float factor_x = (float)originalImage.cols / padding(originalImage.cols, factor);
				if (factor_x != factor){
					factor_x += 0.00001;
				}

				cout << "\nDesired scale factor: " << factor << endl;
				cout << "After padding approximation" << endl;
				cout << "factor_X: " << factor_x << "\tfactor_Y: " << factor_y << endl << endl;
				downsampledImage.rows = padding(originalImage.rows, factor_y);
				downsampledImage.cols = padding(originalImage.cols, factor_x);

				fileSize = downsampledImage.cols*downsampledImage.rows * 3 + 54L;
				downsampledImage.data = (unsigned char *)malloc((downsampledImage.rows*downsampledImage.cols)*sizeof(unsigned char)* 3);
				downsampledImage.header = (unsigned char *)malloc(54L * sizeof(unsigned char));
				memcpy(downsampledImage.header, originalImage.header, 54L);
				memcpy(&downsampledImage.header[2], &fileSize, 4);
				memcpy(&downsampledImage.header[18], &downsampledImage.cols, 4);
				memcpy(&downsampledImage.header[22], &downsampledImage.rows, 4);

				//Setting up memory buffers, initilizing the context from chosen device

				cldownsampler.setup(&originalImage, &downsampledImage, factor_x, factor_y, deviceType);

				printf("Starting downscaling opencl function\n");
				//call to downSampler function in image_downsampling.cpp
				//Setting up kernels arguments, starting computation in opencl, writing back the results in memobject. Here initialization and computation performances are measured.
				cldownsampler.downSampler(tempo);

				printf("\nWriting back results...\n");

				writeImage("DownSampled.bmp", &downsampledImage);

				benchmark << "\nDOWNSCALING 2x: \n" << originalImage.cols * originalImage.rows * 3 << "\t" << fixed << setprecision(5) << tempo << " ms\n" << endl;
				deleteImage(&downsampledImage); deleteImage(&originalImage);
			

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			//MOTION DETECTOR
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*i = -1;
			while (i != 1 && i != 0){
			cout << "insert 0 - CPU \t 1 - GPU: " << endl;
			cin >> i;
			}*/
			if (i == 0)
				deviceType = CL_DEVICE_TYPE_CPU;
			if (i == 1)
				deviceType = CL_DEVICE_TYPE_GPU;

			clMotionDetector.setup(&greyBGImage, &greyMotionImage, &outputImage, deviceType);

			printf("start of motion detection opencl function\n");
			//Setting up kernels arguments, starting computation in opencl, writing back the results in memobject 
			clMotionDetector.motion_detection(tempo);


			printf("\nWriting back results...\n");

			grey2rgb(&motionImage, &outputImage);
			writeImage("Noise.bmp", &motionImage);

			benchmark << "\nMOTION DETECTION: \n" << motionImage.cols * motionImage.rows * 3 << "\t" << fixed << setprecision(5) << tempo << " ms\n" << endl;
			deleteImage(&greyMotionImage); 

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			//EROSION
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*i = -1;
			while (i != 1 && i != 0){
			cout << "insert 0 - CPU \t 1 - GPU: " << endl;
			cin >> i;
			}*/
			if (i == 0)
				deviceType = CL_DEVICE_TYPE_CPU;
			if (i == 1)
				deviceType = CL_DEVICE_TYPE_GPU;

			clErosion.setup(&outputImage, &greyFinalImage, deviceType);

			cout << "\nStarting erosion" << endl;
			clErosion.erosion(tempo);

			grey2rgb(&motionImage, &greyFinalImage);
			writeImage("Finalmotion.bmp", &motionImage);

			benchmark << "\nEROSION: \n" << outputImage.cols * outputImage.rows * 3 << "\t" << fixed << setprecision(5) << tempo << " ms\n" << endl;
			deleteImage(&outputImage); deleteImage(&greyFinalImage); deleteImage(&motionImage);


			/////////////////////////////////////////////////////////////////////////////////////////////////////
			//GAUSSIAN
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*i = -1;
			while (i != 1 && i != 0){
			cout << "insert 0 - CPU \t 1 - GPU: " << endl;
			cin >> i;
			}*/
			if (i == 0)
				deviceType = CL_DEVICE_TYPE_CPU;
			if (i == 1)
				deviceType = CL_DEVICE_TYPE_GPU;

			//function to set up a new mask; Sigma value corresponds to the desired value. It's an user's choise; big sigma takes a long computation time!!!
			gaussianBlur.create1DMask();

			//initiliaze mem objects and the correspondent context
			gaussianBlur.setup(&gaussGreyImage, &gaussOutputImage, deviceType);

			printf("\nStarting gaussian blur opencl function\n");

			//Setting up kernels arguments, starting computation in opencl, writing back the results in memobject. Here performances are measured
			gaussianBlur.blur(tempo);

			printf("\n Writing back results...\n");

			grey2rgb(&gaussImage, &gaussOutputImage);
			writeImage("noise_cat.bmp", &gaussImage);

			benchmark << "\nGAUSSIAN: \n" << gaussImage.cols * gaussImage.rows * 3 << "\t" << fixed << setprecision(5) << tempo << " ms\n" << endl;

			deleteImage(&gaussImage); deleteImage(&gaussOutputImage);


			
			
		}
	}
	printf("enjoy your bitmap!\n");
	benchmark.close();
	system("pause");
	return 0;
}
