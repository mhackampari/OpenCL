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

    if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

    device_id = (cl_device_id *) malloc(num_devices * sizeof (cl_device_id));

    err = clGetDeviceIDs(platforms[0],
            CL_DEVICE_TYPE_ALL,
            num_devices, //queries this amount of devices
            device_id, //list of devices
            NULL); //returns num of present devices

    if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

    for (int i = 0; i < num_devices; i++) {


        size = 0;
        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_AVAILABLE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        device_avbility = (bool *)malloc(size * sizeof (bool));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_AVAILABLE,
                size,
                device_avbility,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\nCL_DEVICE_AVAILABLE: " << device_avbility[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_EXECUTION_CAPABILITIES,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_device_exec_capabilities *device_exec_cap = (cl_device_exec_capabilities *) malloc(size * sizeof (cl_device_exec_capabilities));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_EXECUTION_CAPABILITIES,
                size,
                device_exec_cap,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\nCL_DEVICE_EXECUTION_CAPABILITIES: " << device_exec_cap[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_ulong *device_global_mem_cache_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                size,
                device_global_mem_cache_size,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_SIZE: " << device_global_mem_cache_size[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_device_mem_cache_type *device_global_mem_cache_type = (cl_device_mem_cache_type *) malloc(size * sizeof (cl_device_mem_cache_type));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                size,
                device_global_mem_cache_type,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << device_global_mem_cache_type[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_uint *device_global_mem_cacheline_size = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                size,
                device_global_mem_cacheline_size,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << device_global_mem_cacheline_size[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_SIZE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_ulong *device_global_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_GLOBAL_MEM_SIZE,
                size,
                device_global_mem_size,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_GLOBAL_MEM_SIZE: " << device_global_mem_size[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_LOCAL_MEM_SIZE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_ulong *device_local_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_LOCAL_MEM_SIZE,
                size,
                device_local_mem_size,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_size[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_LOCAL_MEM_TYPE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_device_local_mem_type *device_local_mem_type = (cl_device_local_mem_type *) malloc(size * sizeof (cl_device_local_mem_type));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_LOCAL_MEM_TYPE,
                size,
                device_local_mem_type,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_type[i];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_CLOCK_FREQUENCY,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_uint *device_max_clock_freq = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_CLOCK_FREQUENCY,
                size,
                device_max_clock_freq,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_MAX_CLOCK_FREQUENCY: " << device_max_clock_freq[i];

        /*The number of parallel compute cores on the OpenCL device. The minimum value is 1.*/
        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_COMPUTE_UNITS,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_uint *device_max_compute_units = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_COMPUTE_UNITS,
                size,
                device_max_compute_units,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_MAX_COMPUTE_UNITS: " << device_max_compute_units[i];

        /*Max size of memory object allocation in bytes. 
         * The minimum value is max 
         * (1/4th of CL_DEVICE_GLOBAL_MEM_SIZE, 128*1024*1024)*/
        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_ulong *device_max_mem_alloc_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                size,
                device_max_mem_alloc_size,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_MAX_MEM_ALLOC_SIZE: " << device_max_mem_alloc_size[i];

        /*Maximum number of work-items in a work-group executing a kernel 
         *using the data parallel execution model. 
         * (Refer to clEnqueueNDRangeKernel). 
         * The minimum value is 1*/
        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_WORK_GROUP_SIZE,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        size_t *device_max_work_group_size = (size_t *) malloc(size * sizeof (size_t));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_WORK_GROUP_SIZE,
                size,
                device_max_work_group_size,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_MAX_WORK_GROUP_SIZE: " << device_max_work_group_size[i];

        /*
         * Maximum dimensions that specify the global and local work-item IDs 
         * used by the data parallel execution model. 
         * (Refer to clEnqueueNDRangeKernel). The minimum value is 3.
         */
        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_uint *device_max_work_item_dim = (cl_uint *) malloc(size * sizeof (cl_uint));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                size,
                device_max_work_item_dim,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " << device_max_work_item_dim[i];

        /*DEVICE NAME*/
        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_WORK_ITEM_SIZES,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        size_t *device_max_work_items = (size_t *) malloc(size * sizeof (size_t));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_MAX_WORK_ITEM_SIZES,
                size,
                device_max_work_items,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_MAX_WORK_ITEM_SIZES 1: " << device_max_work_items[0];
        cout << "\n CL_DEVICE_MAX_WORK_ITEM_SIZES 2: " << device_max_work_items[1];
        cout << "\n CL_DEVICE_MAX_WORK_ITEM_SIZES 3: " << device_max_work_items[2];

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_NAME,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cl_device_type *device_type = (cl_device_type *) malloc(size * sizeof (cl_device_type));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_NAME,
                size,
                device_type,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_TYPE: ";


        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_VERSION,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        char *device_version = (char *) malloc(size * sizeof (char));

        err = clGetDeviceInfo(device_id[i],
                CL_DEVICE_VERSION,
                size,
                device_version,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DEVICE_VERSION: " << string(device_version);

        err = clGetDeviceInfo(device_id[i],
                CL_DRIVER_VERSION,
                NULL,
                NULL,
                &size);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        char *driver_version = (char *) malloc(size * sizeof (char));

        err = clGetDeviceInfo(device_id[i],
                CL_DRIVER_VERSION,
                size,
                driver_version,
                NULL);

        if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

        cout << "\n CL_DRIVER_VERSION: " << string(driver_version);

    }
}

void Knapsack::createContextQueue() {
    /*
     * An OpenCL context is created with one or more devices. 
     *  Contexts are used by the OpenCL runtime for managing objects such as 
     *  command-queues, memory, program and kernel objects and for executing 
     *  kernels on one or more devices specified in the context. 
     */


    cl_context_properties context_properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platforms[0], 0};
    context = clCreateContextFromType(
            context_properties,
            CL_DEVICE_TYPE_CPU,
            NULL,
            NULL,
            &err);

    if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

    queue = clCreateCommandQueue(context,
            device_id[0],
            NULL,
            &err);

    if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

}

void Knapsack::createProgramBuild() {
    /*A program object in OpenCL encapsulates the program sources or 
     * a binary file*/

    string sourceKernel, line;


    ofs.open("//home//terminator//NetBeansProjects//KnapsackOCL//HelloWorld_Kernel.cl", ios_base::in);
    if (ofs.is_open()) {
        while (ofs.good()) {
            getline(ofs, line);
            sourceKernel.append(line);

        }

        cout << line << endl;
        ofs.close();
    }

    char *x = (char *) malloc(sizeof (char)*(sourceKernel.length() + 1));
    strcpy(x, sourceKernel.c_str());

    cout << string(x);


    program = clCreateProgramWithSource(context,
            1, //count pointers
            (const char **) &x,
            NULL,
            &err);

    if (err != 0)cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

    err = clBuildProgram(
            program,
            1,
            device_id, // If device_id is NULL value, the program executable is built for all devices associated with program for which a source or binary has been loaded
            NULL, //options
            NULL, //callback function
            &err);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;
        printf("Error building program\n");
        char buffer[4096];
        size_t length;
        clGetProgramBuildInfo(
                program,
                device_id[0],
                CL_PROGRAM_BUILD_LOG,
                sizeof (buffer),
                buffer,
                &length
                );
        printf("%s\n", buffer);
        exit(1);
    }


}

void Knapsack::createKernel() {
    /*A kernel object is an encapsulation of the specify
     * __kernel function along with the arguments that are associated with the 
     * __kernel function when executed. The kernel object is eventually sent to 
     * the command queue for execution.*/
    cl_kernel kernel;
    char *kernelName = NULL;


    kernel = clCreateKernel(
            program, // a valid program object that has been successfully built
            "helloworld", // the name of the kernel declared with __kernel
            &err // error return code
            );

    cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

    err = clSetKernelArg(
            kernel,         //valid kernel object
            0,              //the specific argument of a kernel
            sizeof (cl_mem),//the size of the argument data
            &input_data     //a pointer of data used as the argument
            );
    
     cout << "\n!!!" << appsdk::getOpenCLErrorCodeStr(err) << endl;

}

int main(int argc, char** argv) {


    Knapsack ksack;
    //1.Query the host system for OpenCL platform info
    ksack.queryOclPlatformInfo();
    //2. Query the host for OpenCL devices
    ksack.queryOclDevice();
    //3. Create a context and a queue to associate the OpenCL devices.
    ksack.createContextQueue();
    //4. Create programs that will run on one or more associated devices.
    ksack.createProgramBuild();
    //5. Create memory objects on the host or on the device.

    //6. Copy memory data to the device as needed.
    //7. Provide arguments for the kernels.
    //8. Submit the kernels to the command queue for execution.
    //9. Copy the results from the device to the host
    return 0;
}

