#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "DSamplerHeader.h"
#include "image_processing.h"
#include "CL/CLUtil.hpp"
#include <CL/cl.h>
#include <time.h>

#define ITERATIONS 100


int DSampling::setup(sImage* inputImage, sImage* outputImage, float factor_x, float factor_y, cl_device_type dType)
{
	factor_cols = factor_x;
	factor_rows = factor_y;

	// get width and height of input image
	height = inputImage->rows;
	width = inputImage->cols;

	oheight = outputImage->rows;
	owidth = outputImage->cols; 

	// get the pointer to pixel data
	inputImageData = inputImage->data;
	outputImageData = outputImage->data;

	status = CL_SUCCESS;

	// Have a look at the available platforms and pick a default one.
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

	// The block is to move the declaration of prop closer to its use
	cl_command_queue_properties prop = CL_QUEUE_PROFILING_ENABLE;
	commandQueue = clCreateCommandQueue(
		context,
		devices[sampleArgs->deviceId],
		prop,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateCommandQueue failed.");

	//Set device info of given cl_device_id
	retValue = deviceInfo.setDeviceInfo(devices[sampleArgs->deviceId]);
	CHECK_ERROR(retValue, 0, "SDKDeviceInfo::setDeviceInfo() failed");

	// Create and initialize memory objects

	// Create memory object for input Image
	inputImageBuffer = clCreateBuffer(
		context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		width * height * pixelSize,
		inputImageData,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateBuffer failed. (inputImageBuffer)");

	outputImageBuffer = clCreateBuffer(
		context,
		CL_MEM_READ_WRITE,
		owidth * oheight * pixelSize,
		0,
		&status);
	CHECK_OPENCL_ERROR(status, "clCreateBuffer failed. (outputImageBuffer)");

	return SDK_SUCCESS;
}

void DSampling::downSampler(float &tempo)
{
	cumulative_cmd_time = 0;
	cumulative_processing_time = 0;
	cumulative_total_time = 0;
	setup_cumulative = 0;
	setupKernel("downsample");
	for (int i = 0; i<ITERATIONS; i++){
		runDSampler();
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

int	DSampling::setupKernel(std::string name){

	cl_int status = CL_SUCCESS;

	// create a CL program using the kernel source
	buildProgramData buildData;

	buildData.kernelName = std::string(name + "_Kernel.cl");
	buildData.devices = devices;
	buildData.deviceId = sampleArgs->deviceId;
	buildData.flagsStr = std::string("-g"); //"-g -s \"C:\\User\\Path\""
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

int DSampling::runDSampler()
{
	cl_int status = 0;

	// Set kernel arguments and run kernel
	if (runCLKernelsDSampler() != SDK_SUCCESS)
	{
		return SDK_FAILURE;
	}
	return SDK_SUCCESS;
}

int DSampling::runCLKernelsDSampler()
{
	int M, res2;;
	for (M = (int)deviceInfo.maxWorkGroupSize; M>0; M--){
		if (owidth%M == 0){
			res2 = M;
			break;
		}
	}
	int N, res1;
	for (N = ((int)deviceInfo.maxWorkGroupSize) / res2; N>0; N--){
		if (oheight%N == 0){
			res1 = N;
			break;
		}
	}

	size_t globalThreads[] = { owidth, oheight };
	size_t localThreads[] = { res2, res1 };

	
	// Set appropriate arguments to the kernel
	// input buffer image
	status = clSetKernelArg(
		kernl,
		0,
		sizeof(cl_mem),
		&inputImageBuffer);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (inputImageBuffer)");


	// outBuffer imager
	status = clSetKernelArg(
		kernl,
		1,
		sizeof(cl_mem),
		&outputImageBuffer);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (outputImageBuffer)");

	status = clSetKernelArg(
		kernl,
		2,
		sizeof(int),
		&width);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (width)");

	status = clSetKernelArg(
		kernl,
		3,
		sizeof(int),
		&height);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (factorBuffer)");

	status = clSetKernelArg(
		kernl,
		4,
		sizeof(float),
		&factor_cols);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (factor_x)");

	status = clSetKernelArg(
		kernl,
		5,
		sizeof(float),
		&factor_rows);
	CHECK_OPENCL_ERROR(status, "clSetKernelArg failed. (factor_x)");

	// Enqueue a kernel run call.
	//Uncomment to print out PREFERRED_WORK_GROUP_SIZE_MULTIPLE
	/*
	
	size_t param_value_size;

	size_t wgsize;

	KernelWorkGroupInfo kwgi;
	kwgi.setKernelWorkGroupInfo(kernl, devices[0]);

	status = clGetKernelWorkGroupInfo(kernl,
	devices[0],
	CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
	sizeof(size_t),
	&wgsize,
	NULL);

	std::cout << "CL_KERNEL_WORK_GROUP_SIZE: " << wgsize << std::endl;
	*/

	//Measuring kernel's set-up
	//Start measuring
	setup_start = clock();
	
	status = clEnqueueNDRangeKernel(
		commandQueue,
		kernl,
		2,
		NULL,
		globalThreads,
		NULL,//uncomment and set localThreads to switch to manual mode: localThreads
		0,
		NULL,
		&ndrEvt);
	CHECK_OPENCL_ERROR(status, "clEnqueueNDRangeKernel failed.");

	//Won't proceed ahead if all work-items have not finished processing; Synchronization point
	status = clFinish(commandQueue);
	CHECK_OPENCL_ERROR(status, "clFlush failed.");

	//fetch performance data
	
	setup_end = clock() - setup_start;
	setup_cumulative = (setup_cumulative + setup_end);

	clGetEventInfo(
		ndrEvt,
		CL_EVENT_COMMAND_EXECUTION_STATUS,
		sizeof(cl_int),
		&stat,
		NULL);
	//std::cout << "RefCount: " << refCount << "\t Status:" << stat << std::endl;

	//Measures performances of ndrEvt
	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &time_queued, NULL);
	//Checking ndrEvt's reference count
	clGetEventInfo(
		ndrEvt,
		CL_EVENT_REFERENCE_COUNT,
		sizeof(cl_uint),
		&refCount,
		NULL);
	//std::cout << "\t" << refCount;

	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &time_submit, NULL);
	clGetEventInfo(
		ndrEvt,
		CL_EVENT_REFERENCE_COUNT,
		sizeof(cl_uint),
		&refCount,
		NULL);
	//std::cout << "\t" << refCount;

	single_exec_time = time_submit - time_queued;
	cumulative_cmd_time = (cumulative_cmd_time + single_exec_time);

	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &time_start, NULL);
	clGetEventInfo(
		ndrEvt,
		CL_EVENT_REFERENCE_COUNT,
		sizeof(cl_uint),
		&refCount,
		NULL);
	//std::cout << "\t" << refCount;

	clGetEventProfilingInfo(ndrEvt, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &time_end, NULL);

	clGetEventInfo(
		ndrEvt,
		CL_EVENT_REFERENCE_COUNT,
		sizeof(cl_uint),
		&refCount,
		NULL);
	clGetEventInfo(
		ndrEvt,
		CL_EVENT_COMMAND_EXECUTION_STATUS,
		sizeof(cl_int),
		&stat,
		NULL);
	//std::cout << "RefCount: " << refCount << "\t" << stat << std::endl;
	
	
	single_exec_time = time_end - time_start;
	cumulative_processing_time = (cumulative_processing_time + single_exec_time);

	cumulative_total_time = (time_end - time_queued) + cumulative_total_time;

	status = clReleaseEvent(ndrEvt);
	CHECK_OPENCL_ERROR(status, "clReleaseEvent Failed with Error Code:");
	// Enqueue readBuffer
	
	status = clEnqueueReadBuffer(
		commandQueue,
		outputImageBuffer,
		CL_TRUE,
		0,
		owidth * oheight * pixelSize,
		outputImageData,
		0,
		NULL,
		NULL);
	CHECK_OPENCL_ERROR(status, "clEnqueueReadBuffer failed.");

	return SDK_SUCCESS;
}
