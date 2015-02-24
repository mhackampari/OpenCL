/*
 * File:   knapsack_main.cpp
 * Author: terminator
 *
 * Created on February 21, 2015, 10:07 PM
 */
#include <CL/cl.h>

#include "Knapsack.h"
using namespace std;

/*query the total number of platforms available in the system.
  There can be more than one platform.*/
void Knapsack::queryOclPlatformInfo() {


    //query for the number
    err = clGetPlatformIDs(
            0, // the number of entries that can added to platforms
            NULL, // list of OpenCL found
            &num_platforms); //stores the actual number of platforms present

    //prepare to allocate of the available platforms
    platforms = (cl_platform_id *) malloc(num_platforms * sizeof (cl_platform_id));

    //allocates the available platforms
    err = clGetPlatformIDs(
            num_platforms,
            platforms,
            NULL);

    //prints information for the platforms
    for (int i = 0; i < num_platforms; i++) {
        cout << "*******************\n";
        for (int j = 0; j < sizeof (platform_const_value) / sizeof (cl_platform_info); j++) {


            //retrieve the size of the information for the current platform
            err = clGetPlatformInfo(
                    platforms[0],
                    platform_const_value[j],
                    NULL,
                    NULL,
                    &size);

            //prepare right size for receiving of platform information
            platform_info = (char *) malloc(size * sizeof (char));

            //gets platform information
            err = clGetPlatformInfo(
                    platforms[i], //selected platform
                    platform_const_value[j], //type of information to get
                    size, //size of the info
                    platform_info, //contains the information about platform
                    NULL);

            cout << platform_const_string[j] << ": " << string(platform_info) << "\n";
        }
    }

}

void Knapsack::queryOclDevice() {

    err = clGetDeviceIDs(platforms[0],
            CL_DEVICE_TYPE_ALL,
            NULL, //queries this amount of devices
            NULL, //list of devices
            &num_devices); //returns num of present devices

    device_list = (cl_device_id *) malloc(num_devices * sizeof (cl_device_id));

    err = clGetDeviceIDs(platforms[1],
            CL_DEVICE_TYPE_ALL,
            num_devices, //queries this amount of devices
            device_list, //list of devices
            NULL); //returns num of present devices

    for (int i = 0; i < num_devices; i++) {
        size = 0;
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_AVAILABLE,
                NULL,
                NULL,
                &size);

        device_avbility = (bool *)malloc(size * sizeof (bool));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_AVAILABLE,
                size,
                device_avbility,
                NULL);

        cout << "\nCL_DEVICE_AVAILABLE: " << device_avbility[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_EXECUTION_CAPABILITIES,
                NULL,
                NULL,
                &size);

        cl_device_exec_capabilities *device_exec_cap = (cl_device_exec_capabilities *) malloc(size * sizeof (cl_device_exec_capabilities));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_EXECUTION_CAPABILITIES,
                size,
                device_exec_cap,
                NULL);

        cout << "\nCL_DEVICE_EXECUTION_CAPABILITIES: " << device_exec_cap[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                NULL,
                NULL,
                &size);

        cl_ulong *device_global_mem_cache_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                size,
                device_global_mem_cache_size,
                NULL);

        cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_SIZE: " << device_global_mem_cache_size[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                NULL,
                NULL,
                &size);

        cl_device_mem_cache_type *device_global_mem_cache_type = (cl_device_mem_cache_type *) malloc(size * sizeof (cl_device_mem_cache_type));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                size,
                device_global_mem_cache_type,
                NULL);

        cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << device_global_mem_cache_type[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                NULL,
                NULL,
                &size);

        cl_uint *device_global_mem_cacheline_size = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                size,
                device_global_mem_cacheline_size,
                NULL);

        cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << device_global_mem_cacheline_size[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_SIZE,
                NULL,
                NULL,
                &size);

        cl_ulong *device_global_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_GLOBAL_MEM_SIZE,
                size,
                device_global_mem_size,
                NULL);

        cout << "\n CL_DEVICE_GLOBAL_MEM_SIZE: " << device_global_mem_size[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_LOCAL_MEM_SIZE,
                NULL,
                NULL,
                &size);

        cl_ulong *device_local_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_LOCAL_MEM_SIZE,
                size,
                device_local_mem_size,
                NULL);

        cout << "\n CL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_size[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_LOCAL_MEM_TYPE,
                NULL,
                NULL,
                &size);

        cl_device_local_mem_type *device_local_mem_type = (cl_device_local_mem_type *) malloc(size * sizeof (cl_device_local_mem_type));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_LOCAL_MEM_TYPE,
                size,
                device_local_mem_type,
                NULL);

        cout << "\n CL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_type[i];

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_CLOCK_FREQUENCY,
                NULL,
                NULL,
                &size);

        cl_uint *device_max_clock_freq = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_CLOCK_FREQUENCY,
                size,
                device_max_clock_freq,
                NULL);

        cout << "\n CL_DEVICE_MAX_CLOCK_FREQUENCY: " << device_max_clock_freq[i];

        /*The number of parallel compute cores on the OpenCL device. The minimum value is 1.*/
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_COMPUTE_UNITS,
                NULL,
                NULL,
                &size);

        cl_uint *device_max_compute_units = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_COMPUTE_UNITS,
                size,
                device_max_compute_units,
                NULL);

        cout << "\n CL_DEVICE_MAX_COMPUTE_UNITS: " << device_max_compute_units[i];

        /*Max size of memory object allocation in bytes. 
         * The minimum value is max 
         * (1/4th of CL_DEVICE_GLOBAL_MEM_SIZE, 128*1024*1024)*/
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                NULL,
                NULL,
                &size);

        cl_ulong *device_max_mem_alloc_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                size,
                device_max_mem_alloc_size,
                NULL);

        cout << "\n CL_DEVICE_MAX_MEM_ALLOC_SIZE: " << device_max_mem_alloc_size[i];

        /*Maximum number of work-items in a work-group executing a kernel 
         *using the data parallel execution model. 
         * (Refer to clEnqueueNDRangeKernel). 
         * The minimum value is 1*/
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_WORK_GROUP_SIZE,
                NULL,
                NULL,
                &size);

        size_t *device_max_work_group_size = (size_t *) malloc(size * sizeof (size_t));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_WORK_GROUP_SIZE,
                size,
                device_max_work_group_size,
                NULL);

        cout << "\n CL_DEVICE_MAX_WORK_GROUP_SIZE: " << device_max_work_group_size[i];

        /*
         * Maximum dimensions that specify the global and local work-item IDs 
         * used by the data parallel execution model. 
         * (Refer to clEnqueueNDRangeKernel). The minimum value is 3.
         */
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                NULL,
                NULL,
                &size);

        cl_uint *device_max_work_item_dim = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                size,
                device_max_work_item_dim,
                NULL);

        cout << "\n CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " << device_max_work_item_dim[i];

        /*DEVICE NAME*/
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_NAME,
                NULL,
                NULL,
                &size);

        char *device_name = (char *) malloc(size * sizeof (char));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_NAME,
                size,
                device_name,
                NULL);

        cout << "\n CL_DEVICE_NAME: " << string(device_name);

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_TYPE,
                NULL,
                NULL,
                &size);

        cl_device_type *device_type = (cl_device_type *)malloc(size * sizeof (cl_device_type));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_TYPE,
                size,
                device_type,
                NULL);
        
        cout << "\n CL_DEVICE_TYPE: ";
        switch(*device_type){
            case CL_DEVICE_TYPE_CPU: cout<< "CL_DEVICE_TYPE_CPU"; break;
            case CL_DEVICE_TYPE_GPU: cout<< "CL_DEVICE_TYPE_GPU"; break;
            default: cout<< "OTHER TYPE PF DEVICE\n"; break;
        }
        
        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_VERSION,
                NULL,
                NULL,
                &size);

        char *device_version = (char *) malloc(size * sizeof (char));

        err = clGetDeviceInfo(device_list[i],
                CL_DEVICE_VERSION,
                size,
                device_version,
                NULL);

        cout << "\n CL_DEVICE_VERSION: " << string(device_version);
        
        err = clGetDeviceInfo(device_list[i],
                CL_DRIVER_VERSION,
                NULL,
                NULL,
                &size);

        char *driver_version = (char *) malloc(size * sizeof (char));

        err = clGetDeviceInfo(device_list[i],
                CL_DRIVER_VERSION,
                size,
                driver_version,
                NULL);

        cout << "\n CL_DRIVER_VERSION: " << string(driver_version);

    }
}

int main(int argc, char** argv) {


    Knapsack ksack;
    //1.Query the host system for OpenCL platform info
    ksack.queryOclPlatformInfo();
    //2. Query the host for OpenCL devices
    ksack.queryOclDevice();
    return 0;
}

