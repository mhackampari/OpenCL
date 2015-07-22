/* 
 * File:   Knapsack.h
 * Author: terminator
 *
 * Created on February 21, 2015, 10:14 PM
 */

#ifndef KNAPSACK_H
#define	KNAPSACK_H

#include <cstdlib>
#include <string>
#include <CL/cl.h>
#include<iostream>
#include <fstream>
#include <SDKUtil/SDKUtil.hpp>

#include "TestData.h"
#include <string>

using namespace std;

class Knapsack {
    //variables

    const cl_platform_info platform_const_value[4] = {CL_PLATFORM_PROFILE, CL_PLATFORM_VERSION, CL_PLATFORM_NAME, CL_PLATFORM_VENDOR};
    const string platform_const_string[4] = {"CL_PLATFORM_PROFILE", "CL_PLATFORM_VERSION", "CL_PLATFORM_NAME", "CL_PLATFORM_VENDOR"};

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
    size_t total_elements;
    size_t *local_work_items;
    size_t *global_work_items; //dimension of the buffer
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_mem in_even_mem, out_even_mem, in_odd_mem, out_odd_mem, f0_mem, f1_mem;
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
    int *f1, *f0;
    int numelem, cmax;
    double run_time;
    long chrono_time; 
    float timer_time;
    string device_name_str;

    //initialize variables
public:
    Knapsack(TestData&);
    ~Knapsack();

    void queryOclPlatformInfo(fstream *);
    void queryOclDevice(fstream *);
public:
    int getNumDevices();
    void queryOclDeviceInfo(int, fstream *);
    void createContextQueue(int, fstream *);
    void createProgramBuild(int, fstream *);
    void createMemObjects(fstream *);
    void createKernel();
    size_t getLocalWorkItems(size_t totalElements, size_t localThreads, int device);
    size_t getGlobalWorkItems(size_t globalThreads, size_t localThreads, int device);
    void executeMemObjects(cl_mem f_input, int *f, fstream *);
    void writeBufferToDevice(cl_mem &i, cl_mem &o, int *in, int *out);
    void mapBuffers(cl_mem in, cl_mem out, int* i, int* o);
    void setKernelArgs(cl_mem &i, cl_mem &o, int wk, int vk, int maxelem);
    void readBufferFromDevice(cl_mem &output_mem, int *output, fstream *);
    void executeComputation(int i, fstream *, int local_threads);
    void printResults(fstream *);
    void checkError(cl_int);
    string getErrorCode(int e);
    void even(int w, int v, int i, fstream *);
    void odd(int w, int v, int i, fstream *);
    void CL_CALLBACK myCallBack(cl_event event, cl_int cmd_exec_status, void *user_data);
    void OpenCL_Context_Callback(const char *errinfo, const void *private_info, size_t cb, void *user_data);

};

#endif	/* KNAPSACK_H */
