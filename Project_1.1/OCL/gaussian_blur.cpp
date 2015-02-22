#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "GaussianHeader.h"
#include "CL/CLUtil.hpp"
#include <CL/cl.h>


#define ITERATIONS 100
//Creates mask
void GaussianBlur::createBlurMask() {

	std::cout << "insert sigma value: " << std::endl;
	sigma = 3;

	maskSize = (int)ceil(3.0f*sigma);
	mask = new float[(maskSize * 2 + 1)*(maskSize * 2 + 1)];
	sum = 0.0f;
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
	
}

void GaussianBlur::create1DMask(){

	sigma = 3;

	maskSize = (int)ceil(3.0f*sigma);
	mask = new float[maskSize * 2 + 1];
	sum = 0.0f;
	
		for (int b = -maskSize; b < maskSize + 1; b++) {
			float temp = exp(-((float)(b*b) / (2 * sigma*sigma)));
			sum += temp;
			mask[b + maskSize] = temp;
		}
	
	// Normalize the mask
	for (int i = 0; i < maskSize * 2 + 1; i++)
		mask[i] = mask[i] / sum;
}

int GaussianBlur::setup(sImage* inputImage, sImage* outputImage, cl_device_type dType)
{
	// get width and height of input image
	height = inputImage->rows;
	width = inputImage->cols;

	// get the pointer to pixel data
	inputImageData = inputImage->data;
	outputImageData = outputImage->data;
	
	cl_int status = CL_SUCCESS;
	
	// Have a look at the available platforms and pick either the AMD one if available or a reasonable default.
	cl_platform_id platform = NULL;
	int retValue = getPlatform(platform, sampleArgs->platformId, sampleArgs->isPlatformEnabled());
	CHECK_ERROR(retValue, SDK_SUCCESS, "getPlatform() failed");

	// Display available devices.
	retValue = displayDevices(platform, dType);
	CHECK_ERROR(retValue, SDK_SUCCESS, "displayDevices() failed");

	// If we could find our platform, use it. Otherwise use just available platform.
	cl_context_properties cps[3] =
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)platform,
		0
	};

	context = clCreateContextFromType(
		cps,
		dType,
		NULL,
		NULL,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateContextFromType failed.");

	// getting device on which to run the sample
	status = getDevices(context, &devices, sampleArgs->deviceId, sampleArgs->isDeviceIdEnabled());
	CHECK_ERROR(status, SDK_SUCCESS, "getDevices() failed");

	{
		
		cl_command_queue_properties prop = CL_QUEUE_PROFILING_ENABLE;
		commandQueue = clCreateCommandQueue(
			context,
			devices[sampleArgs->deviceId],
			prop,
			&status);
		CHECK_OPENCL_ERROR(status, "clCreateCommandQueue failed.");
	}

	//Set device info of given cl_device_id
	retValue = deviceInfo.setDeviceInfo(devices[sampleArgs->deviceId]);
	CHECK_ERROR(retValue, 0, "SDKDeviceInfo::setDeviceInfo() failed");

	// Create and initialize memory objects

	// Set Presistent memory only for AMD platform
	cl_mem_flags inMemFlags = CL_MEM_READ_WRITE;

	// Create memory object for input Image
	inputImageBuffer = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY| CL_MEM_COPY_HOST_PTR,
		width * height * greyPixelSize,
		inputImageData,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateBuffer failed. (inputImageBuffer)");

	tempImageBuffer = clCreateBuffer(
		context,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		width * height * greyPixelSize,
		inputImageData,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateBuffer failed. (inputImageBuffer)");

	maskBuffer = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
		(maskSize * 2 + 1)*sizeof(float),//(maskSize * 2 + 1)*sizeof(float),
		mask,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateBuffer failed. (maskBuffer)");

	outputImageBuffer = clCreateBuffer(
		context,
		CL_MEM_READ_WRITE,
		width * height * greyPixelSize,
		0,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateBuffer failed. (outputImageBuffer)");

	return SDK_SUCCESS;
}

void GaussianBlur::blur(float &tempo)
{
	cumulative_cmd_time = 0;
	cumulative_processing_time = 0;
	cumulative_total_time = 0;
	setup_cumulative = 0;

	setupKernel("separable");//or write separable
	for (int i = 0; i<ITERATIONS; i++){
		runBlur();
	}

	//std::fixed;
	std::cout.setf(std::ios::fixed);
	std::cout.precision(5);
	//Average time [ms] = Cumulative_Time/ITERATIONS
	std::cout << "\nSet-up time: " << cumulative_cmd_time / (ITERATIONS * 1000000.0) << std::endl;
	std::cout << "\nProcessing time: " << cumulative_processing_time / (ITERATIONS * 1000000.0) << std::endl;
	std::cout << "\nTotal time OpenCL: " << cumulative_total_time / (ITERATIONS * 1000000.0) << std::endl;
	std::cout << "\nTotal time C++: " << 1000 * (float)setup_cumulative / (CLOCKS_PER_SEC * ITERATIONS) << std::endl;
	tempo = cumulative_processing_time / (ITERATIONS * 1000000.0);
	return;
}


int	GaussianBlur::setupKernel(std::string name){

	cl_int status = CL_SUCCESS;

	// create a CL program using the kernel source
	buildProgramData buildData;

	buildData.kernelName = std::string(name + "_Kernel.cl");
	buildData.devices = devices;
	buildData.deviceId = sampleArgs->deviceId;
	buildData.flagsStr = std::string();
	if (sampleArgs->isLoadBinaryEnabled())
	{
		buildData.binaryName = std::string(sampleArgs->loadBinary.c_str());
	}

	if (sampleArgs->isComplierFlagsSpecified())
	{
		buildData.flagsFileName = std::string(sampleArgs->flags.c_str());
	}

	int retValue = buildOpenCLProgram(program, context, buildData);
	CHECK_ERROR(retValue, 0, "buildOpenCLProgram() failed");

	// get a kernel object handle for a kernel with the given name
	char* charname = &name[0];
	kernl = clCreateKernel(
		program,
		charname,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateKernel failed.");

	status = kernelInfo.setKernelWorkGroupInfo(kernl, devices[sampleArgs->deviceId]);
	CHECK_ERROR(status, SDK_SUCCESS, "kernelInfo.setKernelWorkGroupInfo() failed");

	return SDK_SUCCESS;
}

int GaussianBlur::runBlur()
{
	cl_int status = 0;

	// Set kernel arguments and run kernel
	if (runCLKernelsBlur() != SDK_SUCCESS)
	{
		return SDK_FAILURE;
	}
	return SDK_SUCCESS;
}

int GaussianBlur::runCLKernelsBlur()
{
	int M, res2;;
	for (M = (int)deviceInfo.maxWorkGroupSize; M>0; M--){
		if (width%M == 0){
			res2 = M;
			break;
		}
	}
	int N, res1;
	for (N = ((int)deviceInfo.maxWorkGroupSize) / res2; N>0; N--){
		if (height%N == 0){
			res1 = N;
			break;
		}
	}

	size_t globalThreads[] = { width, height };
	size_t localThreads[] = { res2, res1 };
	// Set appropriate arguments to the kernel

	// input buffer image
	status = clSetKernelArg(
		kernl,
		0,
		sizeof(cl_mem),
		&inputImageBuffer);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (inputImageBuffer)");

	status = clSetKernelArg(
		kernl,
		1,
		sizeof(cl_mem),
		&maskBuffer);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (inputImageBuffer)");

	status = clSetKernelArg(
		kernl,
		2,
		sizeof(int),
		&maskSize);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (maskSize)");

	// outBuffer imager
	status = clSetKernelArg(
		kernl,
		3,
		sizeof(cl_mem),
		&outputImageBuffer);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (outputImageBuffer)");

	status = clSetKernelArg(
		kernl,
		4,
		sizeof(cl_mem),
		&tempImageBuffer);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (outputImageBuffer)");

	// Enqueue a kernel run call.
	//Measuring kernel's set-up
	//Start measuring
	setup_start = clock();
	
	status = clEnqueueNDRangeKernel(
		commandQueue,
		kernl,
		2,
		NULL,
		globalThreads,
		NULL, //uncomment and set localThreads to switch to manual mode: localThreads
		0,
		NULL,
		&ndrEvt);
	CHECK_OPENCL_ERROR(status, "clEnqueueNDRangeKernel failed.");

	//status = clFlush(commandQueue);
	status = clFinish(commandQueue);
	CHECK_OPENCL_ERROR(status, "clFlush failed.");

	//fetch performance data
	setup_end = clock() - setup_start;
	setup_cumulative = (setup_cumulative + setup_end);

	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &time_queued, NULL);
	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &time_submit, NULL);
	single_exec_time = time_submit - time_queued;
	cumulative_cmd_time = (cumulative_cmd_time + single_exec_time);

	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &time_start, NULL);
	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &time_end, NULL);
	single_exec_time = time_end - time_start;
	cumulative_processing_time = (cumulative_processing_time + single_exec_time);

	cumulative_total_time = (time_end - time_queued) + cumulative_total_time;

	status = clReleaseEvent(ndrEvt);
	CHECK_OPENCL_ERROR(status, "clReleaseEvent Failed with Error Code:");

	// Enqueue readBuffer
	//cl_event readEvt;
	status = clEnqueueReadBuffer(
		commandQueue,
		outputImageBuffer,
		CL_TRUE,
		0,
		width * height * greyPixelSize,
		outputImageData,
		0,
		NULL,
		NULL);
	
	CHECK_OPENCL_ERROR(status, "clEnqueueReadBuffer failed.");

	return SDK_SUCCESS;
}
