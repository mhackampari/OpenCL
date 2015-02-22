#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "image_processing.hpp"
#include <ctime>
#include <fstream>
#include <iomanip>

#define ITERATIONS 25

using namespace std;

double factor = 2;

class Image{

private:
	string cat[5];
	string motion[5];
	string background[5];
	int i;

public:
	sImage *inputDS, *outputDS, *motionI, *backgroundI, *outputMD, *outputE, *outputE2, *gaussianI, *gaussianO, *gaussianG, *gaussianGS;

public:
	float factor_x = 0, factor_y = 0;

public: //default constructor
	Image(){
		
		exit;
		
	}

public: //constructor goes through all image files
	Image(int i){

		string cat[5] = { "cat025.bmp", "cat05.bmp", "cat.bmp", "2cat.bmp", "4cat.bmp" }; //downsample
		string motion[5] = { "motion025.bmp", "motion05.bmp", "motion.bmp", "2motion.bmp", "4motion.bmp" };
		string background[5] = { "background025.bmp", "background05.bmp", "background.bmp", "2background.bmp", "4background.bmp" };

		// problema è qui!!!
		inputDS = new sImage;
		outputDS = new sImage;

		motionI = new sImage;
		backgroundI = new sImage;
		outputMD = new sImage;
		outputE = new sImage;
		outputE2 = new sImage;
		gaussianI = new sImage;
		gaussianG = new sImage;
		gaussianO = new sImage;
		gaussianGS = new sImage;


		*inputDS = set_image(cat[i]);
		factor_x = set_factor(inputDS->rows, factor);
		factor_y = set_factor(inputDS->rows, factor);
		*outputDS = set_downscale_output(*inputDS, factor);


		*motionI = set_image(motion[i]);
		*backgroundI = set_image(background[i]);
		
		*outputMD = set_output_image(*backgroundI); // copying properties to all files
		*outputE = set_output_image(*backgroundI);
		*outputE2 = set_output_image(*backgroundI);
		rgb2grey(backgroundI, outputE);
		rgb2grey(motionI, outputE2);

		*gaussianI = set_image(cat[i]);
		*gaussianG = set_image(cat[i]);
		*gaussianGS = set_image(cat[i]);
		rgb2grey(gaussianI, gaussianG);
		rgb2grey(gaussianI, gaussianGS);
		*gaussianO = set_output_image(*gaussianI);
		
	}
	






private:
	sImage set_image(string file_name){
		sImage image;
		readImage((char *)file_name.c_str(), &image);
		return image;
	}

private:
	sImage set_downscale_output(sImage original, float factor){

		int fileSize = 0;
		sImage output;
		float factor_x, factor_y;

		factor_x = set_factor(original.rows, factor);
		factor_y = set_factor(original.cols, factor);

		output.rows = padding_size(original.rows, factor_y);
		output.cols = padding_size(original.cols, factor_x);

		fileSize = output.cols*output.rows * 3 + 54L;
		output.data = (unsigned char *)malloc((output.rows*output.cols)*sizeof(unsigned char)* 3);
		output.header = (unsigned char *)malloc(54L * sizeof(unsigned char));
		output.depth = original.depth;
		memcpy(output.header, original.header, 54L);
		memcpy(&output.header[2], &fileSize, 4);
		memcpy(&output.header[18], &output.cols, 4);
		memcpy(&output.header[22], &output.rows, 4);


		return output;

	}

private:
	float set_factor(int original_size, float factor){

		float temp = (float)original_size / padding_size(original_size, factor);
		if (temp != factor) temp += 0.00001;
		return temp;
	}

private: 
	int padding_size(int original_size, float factor){
		int temp = 0, rest = 0;
		temp = floor(original_size / factor);
		rest = temp * 3 % 4;
		return temp + rest;
	}

private:
	sImage set_output_image(sImage image){
		sImage output;

		output.rows = image.rows;
		output.cols = image.cols;
		output.data = (unsigned char *)malloc(image.rows*image.cols*sizeof(unsigned char));
		output.header = NULL;

		return output;
	}


};

// measures time and writes on file
class Benchmark{ 

public:
	clock_t start = 0, end = 0, total = 0;

public:
	void startt(){

		start = 0; 
		start = clock();
			
	}

public:
	void endt(){

		end = 0;
		end = clock();
		total = end - start;
	}

public:
	void write_to_file(string filename, int i){

		ofstream file;
		float temp = 1000 * (float)total / (CLOCKS_PER_SEC * ITERATIONS);

		file.open(filename + "_" + "benchmark.txt", ios::out | ios::app);

		if (file.is_open()){
			
			file << filename + "_" + to_string(i) << endl << "*********************************" <<endl;
			file << fixed << setprecision(3) << temp << " ms" << endl << endl;

			file.close();
		}
		else cout << "ERROR OPENENING FILE" << endl;

	}
	
	
};


int main()
{

	string downsample_name = "Downsampled_O";
	string motion_name = "Motion_O";
	string erosion_name = "Erosion_O";
	string gaussian_grey_name = "Gaussian_Grey_O";
	string gaussian_separable_name = "Gaussian_Separable_O";


	for (int i = 0; i < 5; i++){
		
		Image var_image(i); //defining constructor
		Benchmark benchmark;
		Image *var = new Image;

		cout << endl << endl;
		
		for (int j = 0; j < ITERATIONS; j++){
			benchmark.endt();
			image_downsample(var_image.inputDS, var_image.outputDS, var_image.factor_x, var_image.factor_y);
			benchmark.endt();
		}
		benchmark.write_to_file(downsample_name, i);
		writeImage((downsample_name + "_" + to_string(i) + ".bmp").c_str(), var_image.outputDS);
		

		cout << "Processing: " << motion_name + "_" + to_string(i) << endl;

		for (int j = 0; j < ITERATIONS; j++){
			benchmark.endt();
			motion_detection(var_image.outputE, var_image.outputE2, var_image.outputMD);
			benchmark.endt();
		}
		grey2rgb(var_image.outputMD, var_image.backgroundI);
		benchmark.write_to_file(motion_name, i);
		writeImage((motion_name + "_" + to_string(i) + ".bmp").c_str(), var_image.backgroundI);
		
		cout << "Processing: " << erosion_name + "_" + to_string(i) << endl;
		
		for (int j = 0; j < ITERATIONS; j++){
			benchmark.endt();
			erosion_filter(var_image.outputMD, var_image.outputE);
			benchmark.endt();
		}
		grey2rgb(var_image.outputE, var_image.motionI);
		benchmark.write_to_file(erosion_name, i);
		writeImage((erosion_name + "_" + to_string(i) + ".bmp").c_str(), var_image.motionI);

		cout << "Processing: " << gaussian_grey_name + "_" + to_string(i) << endl;

		for (int j = 0; j < ITERATIONS; j++){
			benchmark.endt();
			gaussian_grey_filter(var_image.gaussianG, var_image.gaussianO, 3);
			benchmark.endt();
		}
		grey2rgb(var_image.gaussianO, var_image.gaussianG);
		benchmark.write_to_file(gaussian_grey_name, i);
		writeImage((gaussian_grey_name + "_" + to_string(i) + ".bmp").c_str(), var_image.gaussianG);
		
		cout << "Processing: " << gaussian_separable_name + "_" + to_string(i) << endl;
		
		for (int j = 0; j < ITERATIONS; j++){
			benchmark.endt();
			gaussian_separable_filter(var_image.gaussianGS, var_image.gaussianG, var_image.gaussianO, 3);
			benchmark.endt();
		}
		grey2rgb(var_image.gaussianO, var_image.gaussianG);
		benchmark.write_to_file(gaussian_separable_name, i);
		writeImage((gaussian_separable_name + "_" + to_string(i) + ".bmp").c_str(), var_image.gaussianG);
	}

	
	return 0;
}


