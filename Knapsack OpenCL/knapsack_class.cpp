
#include "TestData.h"
#include <math.h>
#include "Knapsack.h"
#include "Timer.h"
#include "Chrono.h"
using namespace std;

void Knapsack::queryOclPlatformInfo(fstream *logfile) {
    num_platforms = 1;
    //prepare to allocate of the available platforms
    platforms = (cl_platform_id *) malloc(num_platforms * sizeof (cl_platform_id));

    //allocates the available platforms
    err = clGetPlatformIDs(
            1, // the number of entries that can added to platforms
            platforms, // list of OpenCL found 
            NULL); //stores the actual number of platforms present

    for (int i = 0; i < num_platforms; i++) {
        cout << "*******************\n";
        *logfile << "*******************\n";
        for (int j = 0; j < sizeof (platform_const_value) / sizeof (cl_platform_info); j++) {

            //retrieve the size of the information for the current platform
            err = clGetPlatformInfo(
                    platforms[i],
                    platform_const_value[j],
                    0,
                    0,
                    &size);

            //prepare right size for receiving of platform information
            platform_info = (char *) calloc(size, sizeof (char));

            //gets platform information
            err = clGetPlatformInfo(
                    platforms[i], //selected platform
                    platform_const_value[j], //type of information to get
                    size, //size of the info
                    platform_info, //contains the information about platform
                    nullptr);

            cout << platform_const_string[j] << ": " << string(platform_info) << "\n";
            *logfile << platform_const_string[j] << ": " << string(platform_info) << "\n";
            free(platform_info);
        }
    }

}

void Knapsack::queryOclDevice(fstream *logfile) {

    err = clGetDeviceIDs(platforms[0],
            CL_DEVICE_TYPE_ALL,
            0, //queries this amount of devices
            0, //list of devices
            &num_devices); //returns num of present devices

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }


    device_id = (cl_device_id *) malloc(num_devices * sizeof (cl_device_id));

    err = clGetDeviceIDs(platforms[0],
            CL_DEVICE_TYPE_ALL,
            num_devices, //queries this amount of devices
            device_id, //list of devices
            NULL); //returns num of present devices

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
}

void Knapsack::queryOclDeviceInfo(int i, fstream *logfile) {

    size = 0;


    err = clGetDeviceInfo(device_id[i],
            CL_DRIVER_VERSION,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    char *driver_version = (char *) malloc(size * sizeof (char));

    err = clGetDeviceInfo(device_id[i],
            CL_DRIVER_VERSION,
            size,
            driver_version,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DRIVER_VERSION: " << string(driver_version);
    *logfile << "\nCL_DRIVER_VERSION: " << string(driver_version);
    free(driver_version);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_NAME,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    char *device_name = (char *) malloc(size * sizeof (char));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_NAME,
            size,
            device_name,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_NAME: " << string(device_name);
    *logfile << "\nCL_DEVICE_NAME: " << string(device_name);
    free(device_name);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_TYPE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_device_type *device_type = (cl_device_type *) malloc(size * sizeof (cl_device_type));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_TYPE,
            size,
            device_type,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_TYPE: ";
    *logfile << "\nCL_DEVICE_TYPE: ";
    switch (*device_type) {
        case(CL_DEVICE_TYPE_GPU): cout << "CL_DEVICE_TYPE_GPU";
            *logfile << "CL_DEVICE_TYPE_GPU";
            break;
        case(CL_DEVICE_TYPE_CPU): cout << "CL_DEVICE_TYPE_CPU";
            *logfile << "CL_DEVICE_TYPE_CPU";
            break;
        default: cout << "OTHER_DEVICE";
            *logfile << "OTHER_DEVICE";
            break;
    }

    free(device_type);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VENDOR,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    char *device_vendor = (char *) malloc(sizeof (char)*size);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VENDOR,
            size,
            device_vendor,
            NULL);

    cout << "\nCL_DEVICE_VENDOR: " << string(device_vendor);
    *logfile << "\nCL_DEVICE_VENDOR: " << string(device_vendor);
    free(device_vendor);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VERSION,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    char *device_version = (char *) malloc(size * sizeof (char));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VERSION,
            size,
            device_version,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_VERSION: " << string(device_version);
    *logfile << "\nCL_DEVICE_VERSION: " << string(device_version);
    free(device_version);


    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_SIZE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_ulong *device_global_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_SIZE,
            size,
            device_global_mem_size,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_GLOBAL_MEM_SIZE: " << device_global_mem_size[0] / pow(10.0, 6) << " MB";
    *logfile << "\nCL_DEVICE_GLOBAL_MEM_SIZE: " << device_global_mem_size[0] / pow(10.0, 6) << " MB";
    free(device_global_mem_size);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_MEM_ALLOC_SIZE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_ulong *device_max_alloc_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_MEM_ALLOC_SIZE,
            size,
            device_max_alloc_mem_size,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\n!!!CL_DEVICE_MAX_MEMORY_ALLOCATION_SIZE: " << *device_max_alloc_mem_size / pow(10.0, 6) << " MB";
    *logfile << "\n!!!CL_DEVICE_MEMORY_ALLOCATION_SIZE: " << *device_max_alloc_mem_size / pow(10.0, 6) << " MB";
    free(device_max_alloc_mem_size);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_ulong *device_global_mem_cache_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
            size,
            device_global_mem_cache_size,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_GLOBAL_MEM_CACHE_SIZE: " << device_global_mem_cache_size[0] / 1000 << "KB";
    *logfile << "\nCL_DEVICE_GLOBAL_MEM_CACHE_SIZE: " << device_global_mem_cache_size[0] / 1000 << "KB";
    free(device_global_mem_cache_size);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_SIZE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_ulong *device_local_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_SIZE,
            size,
            device_local_mem_size,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_size[0] / 1000 << "KB";
    *logfile << "\nCL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_size[0] / 1000 << "KB";
    free(device_local_mem_size);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_TYPE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_device_local_mem_type *device_local_mem_type = (cl_device_local_mem_type *) malloc(size * sizeof (cl_device_local_mem_type));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_TYPE,
            size,
            device_local_mem_type,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_LOCAL_MEM_TYPE: ";
    switch (device_local_mem_type[i]) {
        case(CL_NONE):
            cout << "CL_NONE";
            *logfile << "CL_NONE";
            break;
        case(CL_READ_ONLY_CACHE):
            cout << "CL_READ_ONLY_CACHE";
            *logfile << "CL_NONE";
            break;
        case(CL_READ_WRITE_CACHE):
            cout << "CL_READ_WRITE_CACHE";
            *logfile << "CL_NONE";
            break;
        default:
            cout << "UNKNOWN CACHE TYPE or ERROR";
            *logfile << "UNKNOWN CACHE TYPE or ERROR";
            break;
    };

    *logfile << "\nCL_DEVICE_LOCAL_MEM_TYPE: " << device_local_mem_type[0];
    free(device_local_mem_type);


    /*The number of parallel compute cores on the OpenCL device. The minimum value is 1.*/
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_COMPUTE_UNITS,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cl_uint *device_max_compute_units = (cl_uint *) malloc(size * sizeof (cl_uint));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_COMPUTE_UNITS,
            size,
            device_max_compute_units,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_MAX_COMPUTE_UNITS: " << device_max_compute_units[0];
    *logfile << "\nCL_DEVICE_MAX_COMPUTE_UNITS: " << device_max_compute_units[0];
    free(device_max_compute_units);

    /*Maximum number of work-items in a work-group executing a kernel 
     *using the data parallel execution model. 
     * (Refer to clEnqueueNDRangeKernel). 
     * The minimum value is 1*/
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    device_max_work_group_size = (size_t *) malloc(size * sizeof (size_t));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            size,
            device_max_work_group_size,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_MAX_WORK_GROUP_SIZE: " << device_max_work_group_size[0];
    *logfile << "\nCL_DEVICE_MAX_WORK_GROUP_SIZE: " << device_max_work_group_size[0];
    //free(device_max_work_group_size) is in cleanup function


    /*DEVICE NAME*/
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            0,
            0,
            &size);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    size_t *device_max_work_items = (size_t *) malloc(size * sizeof (size_t));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            size,
            device_max_work_items,
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    cout << "\nCL_DEVICE_MAX_WORK_ITEM_SIZES 1: " << device_max_work_items[0];
    cout << "\nCL_DEVICE_MAX_WORK_ITEM_SIZES 2: " << device_max_work_items[1];
    cout << "\nCL_DEVICE_MAX_WORK_ITEM_SIZES 3: " << device_max_work_items[2];
    *logfile << "\nCL_DEVICE_MAX_WORK_ITEM_SIZES 1: " << device_max_work_items[0];
    *logfile << "\nCL_DEVICE_MAX_WORK_ITEM_SIZES 2: " << device_max_work_items[1];
    *logfile << "\nCL_DEVICE_MAX_WORK_ITEM_SIZES 3: " << device_max_work_items[2];
    free(device_max_work_items);

}

void Knapsack::createContextQueue(int i, fstream *logfile) {
    /*
     * An OpenCL context is created with one or more devices. 
     *  Contexts are used by the OpenCL runtime for managing objects such as 
     *  command-queues, memory, program and kernel objects and for executing 
     *  kernels on one or more devices specified in the context. 
     */


    cl_context_properties context_properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platforms[0], 0};
    context = clCreateContextFromType(
            context_properties,
            CL_DEVICE_TYPE_ALL,
            0,
            0,
            &err);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    queue = clCreateCommandQueue(context,
            device_id[i],
            CL_QUEUE_PROFILING_ENABLE,
            &err);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

}

void Knapsack::createProgramBuild(int i, fstream *logfile) {

    /*A program object in OpenCL encapsulates the program sources or 
     * a binary file*/

    string sourceKernel, line;


    ofs.open("..//OpenCL//Knapsack OpenCL//knapsack_toth.cl", ios_base::in); //..//OpenCL//Knapsack OpenCL//knapsack_toth.cl
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

    cout << "\nKERNEL OUTPUT: \n";
    cout << string(x);
    cout << endl;
    *logfile << "\nKERNEL OUTPUT: \n";
    *logfile << string(x);
    *logfile << endl;


    program = clCreateProgramWithSource(
            context,
            1, //count pointers
            (const char **) &x,
            NULL,
            &err);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    err = clBuildProgram(
            program,
            1,
            &device_id[i], // If device_id is 0 value, the program executable is built for all devices associated with program for which a source or binary has been loaded
            NULL, //options
            NULL, //callback function
            NULL);

    if (err != CL_SUCCESS) {
        printf("Error building program\n");
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "Error building program\n";
        char buffer[4096] = {""};
        size_t length = 4096;
        clGetProgramBuildInfo(
                program,
                device_id[i],
                CL_PROGRAM_BUILD_LOG,
                4096 * sizeof (buffer),
                buffer,
                0
                );
        cout << string(buffer) << "\n";
        *logfile << string(buffer) << "\n";
        logfile->close();

        exit(1);
    }
    free(x);


}

void Knapsack::createMemObjects(fstream *logfile) {

    /*Enqueues a command to write data from host to memory to a buffer object.
     *This functional is typically used to provide data for the kernel processing. 
     */

    in_even_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_ONLY, // bit-field flag to specify;  the usage of memory
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    checkError(err);

    out_even_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_WRITE_ONLY, // bit-field flag to specify
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    checkError(err);

    in_odd_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_ONLY, // bit-field flag to specify;  the usage of memory
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    checkError(err);

    out_odd_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_WRITE_ONLY, // bit-field flag to specify
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    checkError(err);

    m_d_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_WRITE_ONLY, // bit-field flag to specify
            sizeof (int)*capacity, // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    checkError(err);

    /*Enqueues a command to write data from host to memory to a buffer object.
     *This functional is typically used to provide data for the kernel processing. 
     */

}

void Knapsack::createKernel() {

    /*A kernel object is an encapsulation of the specify
     * __kernel function along with the arguments that are associated with the 
     * __kernel function when executed. The kernel object is eventually sent to 
     * the command queue for execution.*/

    kernel = clCreateKernel(
            program, // a valid program object that has been successfully built
            "knapsack", // the name of the kernel declared with __kernel
            &err // error return code
            );
    checkError(err);


}

void Knapsack::writeBufferToDevice(cl_mem& in, cl_mem& out, int* input, int* output) {

    err = clEnqueueWriteBuffer(queue,
            in,
            CL_TRUE,
            0,
            sizeof (capacity)*(capacity + 1),
            input,
            0,
            NULL,
            NULL);
    checkError(err);

    err = clEnqueueWriteBuffer(queue,
            out,
            CL_TRUE,
            0,
            sizeof (capacity)*(capacity + 1),
            output,
            0,
            NULL,
            NULL);
    checkError(err);

    err = clEnqueueWriteBuffer(queue,
            m_d_mem,
            CL_TRUE,
            0,
            sizeof (capacity)*(capacity),
            m_d,
            0,
            NULL,
            NULL);
    checkError(err);

}

void Knapsack::setKernelArgs(cl_mem &input, cl_mem &output, int weightk, int valuek, int total_elements) {

    err = clSetKernelArg(
            kernel, //valid kernel object
            0, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &input //a pointer of data used as the argument
            );
    checkError(err);

    err = clSetKernelArg(
            kernel, //valid kernel object
            1, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &output//a pointer of data used as the argument
            );
    checkError(err);

    err = clSetKernelArg(
            kernel, //valid kernel object
            2, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &m_d_mem //a pointer of data used as the argument
            );
    checkError(err);

    err = clSetKernelArg(
            kernel, //valid kernel object
            3, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &cmax //a pointer of data used as the argument
            );
    checkError(err);

    err = clSetKernelArg(
            kernel, //valid kernel object
            4, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &weightk //a pointer of data used as the argument
            );
    checkError(err);

    err = clSetKernelArg(
            kernel, //valid kernel object
            5, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &valuek //a pointer of data used as the argument
            );
    checkError(err);
    
    err = clSetKernelArg(
            kernel, //valid kernel object
            6, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &total_elements //a pointer of data used as the argument
            );
    checkError(err);

}

void Knapsack::even(int weightk, int valuek, int i, fstream* logfile) {

    /*
     * Enqueues a command to execute a kernel on a device.
     * 
     * OpenCL allows work-items to be combined into work-groups, and all work-items
     * within a work-group are executed on the same compute unit on the same device.
     * When a work-group size is specified, OpenCL divides the global work size among 
     * the compute units on the device. In addition, work-items within a work-group
     * execute synchronously and are able to share memory. The total number of
     * work-items  in a work-group is known as the local work size. Group seizes
     * cannot be assigned arbitrarily; the provided OpenCL function clGetKernelWorkGroupInfo()
     * must be used to query the group size info of a device.
     */

    err = clEnqueueNDRangeKernel(
            queue, // valid command queue
            kernel, // valid kernel object
            1, // the work problem dimensions
            NULL, //must be NULL
            global_work_items, // work-items for each dimension
            local_work_items, // work-group size for each dimension
            0, // number of event in the event list
            NULL, // list of events that needs to complete before this executes
            &prof_event // event object to return on completion
            );
    checkError(err);

    err = clFinish(queue);
    checkError(err);

    err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_QUEUED, sizeof (cl_ulong), &start_time, NULL);
    checkError(err);

    err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end_time, NULL);
    checkError(err);

    run_time += (double) (end_time - start_time) * pow(10, -6); //time in ms

}

void Knapsack::odd(int weightk, int valuek, int i, fstream* logfile) {

    /*
     * Enqueues a command to execute a kernel on a device.
     * 
     * OpenCL allows work-items to be combined into work-groups, and all work-items
     * within a work-group are executed on the same compute unit on the same device.
     * When a work-group size is specified, OpenCL divides the global work size among 
     * the compute units on the device. In addition, work-items within a work-group
     * execute synchronously and are able to share memory. The total number of
     * work-items  in a work-group is known as the local work size. Group seizes
     * cannot be assigned arbitrarily; the provided OpenCL function clGetKernelWorkGroupInfo()
     * must be used to query the group size info of a device.
     */

    err = clEnqueueNDRangeKernel(
            queue, // valid command queue
            kernel, // valid kernel object
            1, // the work problem dimensions
            NULL, //must be NULL
            global_work_items, // work-items for each dimension
            local_work_items, // work-group size for each dimension
            0, // number of event in the event list
            NULL, // list of events that needs to complete before this executes
            &prof_event // event object to return on completion
            );
    checkError(err);

    err = clFinish(queue);
    checkError(err);

    err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_QUEUED, sizeof (cl_ulong), &start_time, NULL);
    checkError(err);

    err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end_time, NULL);
    checkError(err);

    run_time += (double) (end_time - start_time) * pow(10, -6); //time in ms

}

void Knapsack::readBufferFromDevice(cl_mem &output_mem, int* output, fstream *logfile) {

    /* Enqueues a command to read data from a buffer object into host memory.
     * This function is typically used to READ RESULTS FROM the KERNEL execution
     * back to host program. 
     * Reads the cl memory "output_mem" on device to the host variable "f".
     */

    err = clEnqueueReadBuffer(
            queue, //valid command queue 
            m_d_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*capacity, //size in bytes of data to write to
            m_d, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    err = clEnqueueReadBuffer(
            queue, //valid command queue 
            output_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            output, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        *logfile << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
}

Knapsack::Knapsack(TestData& t) {

    numelem = t.getNumelem();
    sumWeight = t.getSum();
    capacity = t.getCapacity();
    value = (int *) calloc(numelem, sizeof (int));
    weight = (int *) calloc(numelem, sizeof (int));
    memcpy(value, t.getValue(), sizeof (int)*numelem);
    memcpy(weight, t.getWeight(), sizeof (int)*numelem);
    f1 = (int *) calloc((capacity + 1), sizeof (int)); //f[capacity +1]
    f0 = (int *) calloc((capacity + 1), sizeof (int)); //f[0, ..., capacity]
    M = (int *) calloc(numelem * capacity, sizeof (int)); //M[numelem][capacity]
    if (M == NULL) cout << "NOT ENOUGH AVAILABLE SPACE!!!! TRY WITH LESS ELEMENTS\n";
    m_d = (int *) calloc(capacity, sizeof (int)); //m_d[1, ..., capacity] this a row of M matrix
    global_work_items = (size_t *) calloc(1, sizeof (size_t));
    local_work_items = (size_t *) calloc(1, sizeof (size_t));

}

void Knapsack::executeComputation(int i, fstream *logfile) {
    int sumK = sumWeight;
    Timer timer;
    Chrono chrono;
    timer.start();
    chrono.startChrono();
    run_time = 0;
    for (int k = 0; k < numelem; k++) {

        sumK = sumK - weight[k];
        cmax = 0;
        cmax = max(capacity - sumK, weight[k]);
        total_elements = (size_t) (capacity - cmax + 1);
        *global_work_items = total_elements;
        *local_work_items = getLocalWorkItems(total_elements, *device_max_work_group_size, i);
        *global_work_items = getGlobalWorkItems(total_elements, *local_work_items, i);
        
        *logfile << "global_work_items: " << *global_work_items << endl;
        *logfile << "local_work_items: " << *local_work_items << endl;
        *logfile << endl;

        if (k % 2 == 0) {
            writeBufferToDevice(in_even_mem, out_even_mem, f0, f1);
            setKernelArgs(in_even_mem, out_even_mem, weight[k], value[k], (int)total_elements);
            even(weight[k], value[k], i, logfile);
            readBufferFromDevice(out_even_mem, f1, logfile);

        } else {
            writeBufferToDevice(in_odd_mem, out_odd_mem, f1, f0);
            setKernelArgs(in_odd_mem, out_odd_mem, weight[k], value[k], (int)total_elements);
            odd(weight[k], value[k], i, logfile);
            readBufferFromDevice(out_odd_mem, f0, logfile);
        }

        //segmentation fault can be caused because of max memory limit(ram)
        memcpy(M + k*capacity, m_d, sizeof (int)*capacity);

        for (int i = 0; i < capacity; i++) {
            *(m_d + i) = 0;
        }

    }
    timer.stop();
    chrono.stopChrono();
    cout << "Total time with Timer: " << timer.getTime() << endl;
    *logfile << "Total time with Timer: " << timer.getTime() << endl;
    cout << "Total time with Chrono: " << chrono.getTimeChrono() << endl;
    *logfile << "Total time with Chrono: " << chrono.getTimeChrono() << endl;
    timer.reset();

}

void Knapsack::printResults(fstream *logfile) {

    cout << "\nPRINTOUT OF THE RESULTS: " << endl;
    *logfile << "\nPRINTOUT OF THE RESULTS: " << endl;
    cout << "*************************************************" << endl;
    *logfile << "*************************************************" << endl;

    /*
         cout << "Evolution of the Knapsack worth F[x]: ";
         //*logfile << "Evolution of the Knapsack worth F[x]: ";
        for (int i = 0; i < capacity + 1; i++) {
            if (numelem % 2 == 0) {
                cout << f0[i] << " ";
     *logfile << f0[i] << " ";
            } else {
                cout << *(f1 + i) << " ";
     *logfile << *(f1 + i) << " ";
            }

        }

        cout << endl;
        cout << "Matrix of decisions M[items][capacity]: " << endl;
     *logfile << endl;
     *logfile << "Matrix of decisions M[items][capacity]: " << endl;
        for (int i = 0; i < numelem * capacity; i++) {
            cout << *(M + i) << "; ";
     *logfile << *(M + i) << "; ";
            if ((i + 1) % (capacity) == 0) {
                cout << endl;
        // *logfile << endl;
            }
        }*/

    cout << "SumWeight: " << sumWeight << "\t Capacity: " << capacity << "\n";
    cout << "\nKnapsack's worth: ";
    *logfile << "\nKnapsack's worth: ";
    if (numelem % 2 == 0) {
        cout << f0[capacity] << endl;
        *logfile << f0[capacity] << endl;
    } else {
        cout << f1[capacity] << endl;
        *logfile << f1[capacity] << endl;
    }
    cout << endl;

    cout << "Chosen items are:" << endl;
    *logfile << "Chosen items are:" << endl;
    int cap = capacity;
    int capacita = 0;
    for (int item = numelem - 1; item > -1; item--) {
        for (int c = cap - 1; c > -1; c--) {
            if (M[item * (capacity) + c] != 0) {
                cout << "Value " << item << ": " << value[item];
                cout << "\t" << "Weight " << item << ": " << weight[item] << endl;
                capacita += weight[item];
                *logfile << "Value: " << value[item];
                *logfile << "\t" << "Weight: " << weight[item] << endl;
                cap = cap - weight[item];
                break;
            } else if (M[item * (capacity) + c] == 0) {
                c = 0;
            }
        }

    }

    cout << "\nCAPACITA: " << capacita << endl;
    for (int i = 0; i < capacity + 1; i++) {
        *(f1 + i) = 0; //f[i] = 0;
        *(f0 + i) = 0;
    }

    cout << "Profiling Info: " << run_time << endl;
    *logfile << "Profiling Info: " << run_time << endl;


    cout << "*************************************************" << endl;
    cout << "END OF THE PRINTOUT" << endl;
    *logfile << "*************************************************" << endl;
    *logfile << "END OF THE PRINTOUT" << endl;

}

//http://stackoverflow.com/questions/3957125/questions-about-global-and-local-work-size

size_t Knapsack::getLocalWorkItems(size_t totalThreads, size_t localThreads, int i) {

    /*each kernel execution in OpenCL is called a work-item. 
     * OpenCL exploits parallel computation of the compute devices by having 
     * instances of the kernel execute on different portions of the 
     * N-dimensional problem space. In addition, each work-item is executed 
     * only with its assigned data. Thus, it is important specify to OpenCL 
     * how many work-items are needed to process all data.*/

    //http://stackoverflow.com/questions/3957125/questions-about-global-and-local-work-size
    err = clGetKernelWorkGroupInfo(kernel,
            device_id[i],
            CL_KERNEL_WORK_GROUP_SIZE,
            sizeof (size_t),
            &size,
            NULL);
    checkError(err);

    //cout << "\nCL_KERNEL_WORK_GROUP_SIZE: " << size << endl;
    //cout << "\nGLOBAL_WORK_GROUP_SIZE: " << *global_work_items << endl;
    //cout << "\nLOCAL_WORK_GROUP_SIZE: " << *local_work_items << endl;

    /* Each kernel execution in OpenCL is called a work-item thus is important 
     * specify to OpenCL how many work-items are needed to process all data.
     * 
     */
    if (localThreads < totalThreads) {

        return localThreads;

    } else {

        return totalThreads;

    }

}

size_t Knapsack::getGlobalWorkItems(size_t globalThreads, size_t localThreads, int i) {

    /*each kernel execution in OpenCL is called a work-item. 
     * OpenCL exploits parallel computation of the compute devices by having 
     * instances of the kernel execute on different portions of the 
     * N-dimensional problem space. In addition, each work-item is executed 
     * only with its assigned data. Thus, it is important specify to OpenCL 
     * how many work-items are needed to process all data.*/

    /* Each kernel execution in OpenCL is called a work-item thus is important 
     * specify to OpenCL how many work-items are needed to process all data.
     * 
     */

    if (localThreads < globalThreads && globalThreads % localThreads != 0) {

        //Workgroup should be full of threads to have max throughput
        globalThreads = (globalThreads / localThreads + 1) * localThreads;

    }

    //cout << "\nGLOBAL_WORK_GROUP_SIZE: " << globalThreads << endl;
    //cout << "\nLOCAL_WORK_GROUP_SIZE: " << localThreads << endl;

    return globalThreads;
}

int Knapsack::getNumDevices() {
    return (int) num_devices;
}

Knapsack::~Knapsack() {

    err = clReleaseMemObject(in_even_mem);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
    err = clReleaseMemObject(out_even_mem);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
    err = clReleaseMemObject(in_odd_mem);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
    err = clReleaseMemObject(out_odd_mem);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
    err = clReleaseMemObject(m_d_mem);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    err = clReleaseProgram(program);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    err = clReleaseContext(context);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    err = clReleaseCommandQueue(queue);
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }

    free(platforms);
    free(device_id);
    free(M);
    free(value);
    free(weight);
    free(device_max_work_group_size);
    free(f0);
    free(f1);
    free(m_d);
    free(global_work_items);
    free(local_work_items);

    cout << "\n!!!Knapsack Class Has Been Destroyed!!!\n";
}

void Knapsack::checkError(cl_int e) {
    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    }
}

string Knapsack::getErrorCode(int e) {

    int errorCode = (int) e;
    switch (errorCode) {
        case CL_DEVICE_NOT_FOUND:
            return "CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE:
            return "CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE:
            return "CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES:
            return "CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY:
            return "CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP:
            return "CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH:
            return "CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE:
            return "CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE:
            return "CL_MAP_FAILURE";
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case CL_INVALID_VALUE:
            return "CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE:
            return "CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM:
            return "CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE:
            return "CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT:
            return "CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES:
            return "CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE:
            return "CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR:
            return "CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT:
            return "CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE:
            return "CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER:
            return "CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY:
            return "CL_INVALID_BINARY";
        case CL_INVALID_BUILD_OPTIONS:
            return "CL_INVALID_BUILD_OPTIONS";
        case CL_INVALID_PROGRAM:
            return "CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return "CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME:
            return "CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION:
            return "CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL:
            return "CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX:
            return "CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE:
            return "CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE:
            return "CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS:
            return "CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION:
            return "CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE:
            return "CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE:
            return "CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET:
            return "CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST:
            return "CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT:
            return "CL_INVALID_EVENT";
        case CL_INVALID_OPERATION:
            return "CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT:
            return "CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE:
            return "CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL:
            return "CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE:
            return "CL_INVALID_GLOBAL_WORK_SIZE";
        default:
            return "unknown error code";
    }
