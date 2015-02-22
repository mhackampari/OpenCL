#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "CL/CLUtil.hpp"
#include "image_processing.h"

using namespace appsdk;

#define SAMPLE_VERSION "AMD-APP-SDK-v2.9.233.1"

#define INPUT_FILE "due.bmp"
#define OUTPUT_FILE_CL "output_cl.bmp"


class MotionDetector
{

	cl_uchar* inputImageData;          /* Input bitmap data to device */
	cl_uchar* finalImageData;            /* Output from device executing kernel 1 (rgb2grey) */
	cl_uchar* outputImageData;  
	cl_uchar* motionImageData;/* Output from device executing kernel 2 (edgeDetection) */

	cl_mem inputImageBuffer;            /* CL memory buffer for input Image */
	cl_mem motionImageBuffer;
	cl_mem outputImageBuffer;           /* CL memory buffer for final output Image */
	cl_mem finalImageBuffer;				/* CL memory buffer for intermediate output Image */
	cl_device_type dType;
	

	cl_uchar* greyPixelData;			/* Pointer to grey image data */
	cl_uint pixelSize;                  /* Size of a pixel in BMP format RGB */
	cl_uint greyPixelSize;				/* Size of a pixel in BMP format GreyScale */
	cl_uint width;                      /* Width of image */
	cl_uint height;                     /* Height of image */

	cl_context context;                 /* CL context */
	cl_device_id *devices;              /* CL device list */
	cl_command_queue commandQueue;      /* CL command queue */
	cl_program program;                 /* CL program  */
	cl_kernel kernl;                   /* CL kernel */

	size_t kernelWorkGroupSize;         /* Group Size returned by kernel */
	SDKDeviceInfo deviceInfo;           /* Structure to store device information*/
	KernelWorkGroupInfo kernelInfo;     /* Structure to store kernel related info */

	cl_ulong cumulative_processing_time, cumulative_cmd_time, cumulative_total_time, cumulative_time;
	cl_ulong time_start, time_queued;
	cl_ulong time_end, time_submit;
	cl_ulong single_exec_time;
	clock_t setup_start, setup_end, setup_cumulative;

	cl_event ndrEvt;
	cl_int status;

public:

	CLCommandArgs   *sampleArgs;        /* CLCommand argument class */

	// Constructor
	MotionDetector()
		: inputImageData(NULL),
		outputImageData(NULL)
	{
		sampleArgs = new CLCommandArgs();
		sampleArgs->sampleVerStr = SAMPLE_VERSION;
		pixelSize = sizeof(unsigned char)* 3;
		greyPixelSize = sizeof(unsigned char);
	}

	~MotionDetector()
	{
	}

	int runCLKernelsMotion();

	void motion_detection(float &tempo);
	
	int setup(sImage* inputImage, sImage* motionImage, sImage* outputImage, cl_device_type deviceType );

	int runMotion();

	int	setupKernel(std::string name);

	double getCumulativeTime();

	void resetCumulativeTime();

};
