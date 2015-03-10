/* 
 * File:   Knapsack.h
 * Author: terminator
 *
 * Created on February 21, 2015, 10:14 PM
 */

#ifndef KNAPSACK_H
#define	KNAPSACK_H



#endif	/* KNAPSACK_H */

#include <cstdlib>
#include <string>
#include<CL/cl.h>
#include<iostream>
#include <fstream>
#include <SDKUtil/CLUtil.hpp>

using namespace std;

class Knapsack{
    //variables
    
    const cl_platform_info platform_const_value[5] = {CL_PLATFORM_PROFILE, CL_PLATFORM_VERSION, CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_EXTENSIONS};
    const std::string platform_const_string[5] = {"CL_PLATFORM_PROFILE", "CL_PLATFORM_VERSION", "CL_PLATFORM_NAME", "CL_PLATFORM_VENDOR", "CL_PLATFORM_EXTENSIONS"};
    
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_int err;
    char *platform_info;
    size_t size;
    cl_uint num_entries;
    cl_device_id *device_id;
    cl_uint num_devices;
    bool *device_avbility;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_mem value_mem, weight_mem;
    cl_mem f_mem;
    cl_kernel kernel;
    ifstream ofs;
    
    int value[3] = {1, 3, 2};
    int weight[3] = {2, 3, 2};
    int sumWeight = 7;
    int capacity = 5;
    int f[6] = {0, 0, 0, 0, 0, 0};
    
    //initialize variables
    public:Knapsack();

    void queryOclPlatformInfo();
    void queryOclDevice();
    void createContextQueue();
    void createProgramBuild();
    void createMemObjects();
    void createKernel();
    void createExecModelMemObjects();
    void cleanup();
    void CL_CALLBACK myCallBack(cl_event event, cl_int cmd_exec_status, void *user_data);
    void OpenCL_Context_Callback(const char *errinfo, const void *private_info, size_t cb, void *user_data);
    
};