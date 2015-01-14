#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "CL/CLUtil.hpp"
#include "image_processing.h"

using namespace appsdk;

#define SAMPLE_VERSION "AMD-APP-SDK-v2.9.233.1"



class DSampling
{
	cl_int status;
	cl_int stat;

	cl_uchar* inputImageData;          /* Input bitmap data to device */
	cl_uchar* outputImageData;

	cl_mem inputImageBuffer;            /* CL memory buffer for input Image */
	cl_mem outputImageBuffer;
	cl_mem widthBuffer, heightBuffer, factorBuffer;
	/* CL memory buffer for final output Image */
	/* CL memory buffer for intermediate output Image */
	cl_device_type dType;

	float factor_rows, factor_cols;
				
	cl_uint pixelSize;                  /* Size of a pixel in BMP format RGB */
				
	cl_uint width;                      /* Width of image */
	cl_uint height;                     /* Height of image */
	cl_uint owidth;
	cl_uint oheight;
	cl_uint refCount;

	cl_context context;                 /* CL context */
	cl_device_id *devices;              /* CL device list */
	cl_command_queue commandQueue;      /* CL command queue */
	cl_program program;                 /* CL program  */
	cl_kernel kernl;                   /* CL kernel */

	size_t kernelWorkGroupSize;         /* Group Size returned by kernel */
	SDKDeviceInfo deviceInfo;           /* Structure to store device information*/
	KernelWorkGroupInfo kernelInfo;     /* Structure to store kernel related info */

	//variables for performance measuring
	cl_ulong cumulative_processing_time, cumulative_cmd_time, cumulative_total_time, cumulative_time;
	cl_ulong time_start, time_queued;
	cl_ulong time_end, time_submit;
	cl_ulong single_exec_time;
	clock_t setup_start, setup_end, setup_cumulative;

	cl_event ndrEvt;

public:

	CLCommandArgs   *sampleArgs;        /* CLCommand argument class */

	// Constructor
	DSampling()
		: inputImageData(NULL),
		outputImageData(NULL)
	{
		sampleArgs = new CLCommandArgs();
		sampleArgs->sampleVerStr = SAMPLE_VERSION;
		pixelSize = sizeof(unsigned char)* 3;
		
	}

	~DSampling()
	{
	}

	int setup(sImage* inputImage, sImage* outputImage, float factor_cols, float factor_rows, cl_device_type dType);

	void downSampler(float &tempo);

	int	setupKernel(std::string name);

	int runDSampler();

	int runCLKernelsDSampler();

	double getCumulativeTime();

	void resetCumulativeTime();

};
