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
#include "SDKUtil/CLUtil.hpp"

using namespace std;

class Knapsack{
    //variables
    
    const cl_platform_info platform_const_value[5] = {CL_PLATFORM_PROFILE, CL_PLATFORM_VERSION, CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_EXTENSIONS};
    const string platform_const_string[5] = {"CL_PLATFORM_PROFILE", "CL_PLATFORM_VERSION", "CL_PLATFORM_NAME", "CL_PLATFORM_VENDOR", "CL_PLATFORM_EXTENSIONS"};
    
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_int err;
    char *platform_info;
    size_t size;
    cl_uint num_entries;
    cl_device_id *device_id;
    cl_uint num_devices;
    bool *device_avbility;
    size_t *device_max_work_group_size;
    size_t *local_work_items;
    size_t *global_work_items; //dimension of the buffer
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_mem f1_mem, f0_mem;
    cl_mem m_d_mem;
    cl_kernel kernel;
    cl_event prof_event;
    cl_ulong start_time, end_time;
    ifstream ofs;
    
    //int M[3][5];
    int *M, *m_d;
    int *value;
    int *weight;
    int sumWeight;
    int capacity;
    int *f1, *f0, *F;
    int numelem, cmax;
    
    //initialize variables
    public:Knapsack();

    void queryOclPlatformInfo(fstream *);
    void queryOclDevice(fstream *);
public: int getNumDevices(); 
    void queryOclDeviceInfo(int i, fstream *);
    void createContextQueue(int i, fstream *);
    void createProgramBuild(int i, fstream *);
    void createMemObjects(fstream *);
    void createKernel(cl_mem &input, cl_mem &output, int weight, int value, fstream *);
    size_t getLocalWorkItems(size_t globalWorkItems, size_t max_work_group_items);
    void executeMemObjects(cl_mem f_input, int *f, fstream *);
    void readback(cl_mem &output_mem, int *output, fstream *);
    void executeComputation(int i, fstream *);
    void printResults(fstream *);
    string getErrorCode(int e);
    void cleanup();
    void pari(int w, int v, int i, fstream *);
    void dispari(int w, int v, int i, fstream *);
    void CL_CALLBACK myCallBack(cl_event event, cl_int cmd_exec_status, void *user_data);
    void OpenCL_Context_Callback(const char *errinfo, const void *private_info, size_t cb, void *user_data);
    
};