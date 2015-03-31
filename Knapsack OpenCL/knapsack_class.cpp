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

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    device_id = (cl_device_id *) malloc(num_devices * sizeof (cl_device_id));

    err = clGetDeviceIDs(platforms[0],
            CL_DEVICE_TYPE_ALL,
            num_devices, //queries this amount of devices
            device_id, //list of devices
            NULL); //returns num of present devices

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
}

void Knapsack::queryOclDeviceInfo(int i) {

    size = 0;
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_AVAILABLE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    device_avbility = (bool *)malloc(size * sizeof (bool));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_AVAILABLE,
            size,
            device_avbility,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\nCL_DEVICE_AVAILABLE: " << device_avbility[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_EXECUTION_CAPABILITIES,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_device_exec_capabilities *device_exec_cap = (cl_device_exec_capabilities *) malloc(size * sizeof (cl_device_exec_capabilities));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_EXECUTION_CAPABILITIES,
            size,
            device_exec_cap,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\nCL_DEVICE_EXECUTION_CAPABILITIES: " << device_exec_cap[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_ulong *device_global_mem_cache_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
            size,
            device_global_mem_cache_size,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_SIZE: " << device_global_mem_cache_size[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_device_mem_cache_type *device_global_mem_cache_type = (cl_device_mem_cache_type *) malloc(size * sizeof (cl_device_mem_cache_type));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
            size,
            device_global_mem_cache_type,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << device_global_mem_cache_type[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_uint *device_global_mem_cacheline_size = (cl_uint *) malloc(size * sizeof (cl_uint));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
            size,
            device_global_mem_cacheline_size,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_GLOBAL_MEM_CACHE_TYPE: " << device_global_mem_cacheline_size[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_SIZE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_ulong *device_global_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_GLOBAL_MEM_SIZE,
            size,
            device_global_mem_size,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_GLOBAL_MEM_SIZE: " << device_global_mem_size[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_SIZE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_ulong *device_local_mem_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_SIZE,
            size,
            device_local_mem_size,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_size[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_TYPE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_device_local_mem_type *device_local_mem_type = (cl_device_local_mem_type *) malloc(size * sizeof (cl_device_local_mem_type));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_LOCAL_MEM_TYPE,
            size,
            device_local_mem_type,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_LOCAL_MEM_SIZE: " << device_local_mem_type[i];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_CLOCK_FREQUENCY,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_uint *device_max_clock_freq = (cl_uint *) malloc(size * sizeof (cl_uint));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_CLOCK_FREQUENCY,
            size,
            device_max_clock_freq,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_MAX_CLOCK_FREQUENCY: " << device_max_clock_freq[i];

    /*The number of parallel compute cores on the OpenCL device. The minimum value is 1.*/
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_COMPUTE_UNITS,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_uint *device_max_compute_units = (cl_uint *) malloc(size * sizeof (cl_uint));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_COMPUTE_UNITS,
            size,
            device_max_compute_units,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_MAX_COMPUTE_UNITS: " << device_max_compute_units[i];

    /*Max size of memory object allocation in bytes. 
     * The minimum value is max 
     * (1/4th of CL_DEVICE_GLOBAL_MEM_SIZE, 128*1024*1024)*/
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_MEM_ALLOC_SIZE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_ulong *device_max_mem_alloc_size = (cl_ulong *) malloc(size * sizeof (cl_ulong));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_MEM_ALLOC_SIZE,
            size,
            device_max_mem_alloc_size,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

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

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    device_max_work_group_size = (size_t *) malloc(size * sizeof (size_t));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            size,
            device_max_work_group_size,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

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

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_uint *device_max_work_item_dim = (cl_uint *) malloc(size * sizeof (cl_uint));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
            size,
            device_max_work_item_dim,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS: " << device_max_work_item_dim[i];

    /*DEVICE NAME*/
    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    size_t *device_max_work_items = (size_t *) malloc(size * sizeof (size_t));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_MAX_WORK_ITEM_SIZES,
            size,
            device_max_work_items,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_MAX_WORK_ITEM_SIZES 1: " << device_max_work_items[0];
    cout << "\n CL_DEVICE_MAX_WORK_ITEM_SIZES 2: " << device_max_work_items[1];
    cout << "\n CL_DEVICE_MAX_WORK_ITEM_SIZES 3: " << device_max_work_items[2];

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_NAME,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    char *device_name = (char *) malloc(size * sizeof (char));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_NAME,
            size,
            device_name,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_NAME: " << device_name;

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_TYPE,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cl_device_type *device_type = (cl_device_type *) malloc(size * sizeof (cl_device_type));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_TYPE,
            size,
            device_type,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_TYPE: ";
    switch (*device_type) {
        case(CL_DEVICE_TYPE_GPU): cout << "CL_DEVICE_TYPE_GPU";
            break;
        case(CL_DEVICE_TYPE_CPU): cout << "CL_DEVICE_TYPE_CPU";
            break;
        default: cout << "OTHER_DEVICE";
    }

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VENDOR,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    char *device_vendor = (char *) malloc(sizeof (char)*size);

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VENDOR,
            size,
            device_vendor,
            NULL);

    cout << "\n CL_DEVICE_VENDOR: " << string(device_vendor);


    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VERSION,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    char *device_version = (char *) malloc(size * sizeof (char));

    err = clGetDeviceInfo(device_id[i],
            CL_DEVICE_VERSION,
            size,
            device_version,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DEVICE_VERSION: " << string(device_version);

    err = clGetDeviceInfo(device_id[i],
            CL_DRIVER_VERSION,
            NULL,
            NULL,
            &size);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    char *driver_version = (char *) malloc(size * sizeof (char));

    err = clGetDeviceInfo(device_id[i],
            CL_DRIVER_VERSION,
            size,
            driver_version,
            NULL);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "\n CL_DRIVER_VERSION: " << string(driver_version);

}

void Knapsack::createContextQueue(int i) {
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
            NULL,
            NULL,
            &err);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    queue = clCreateCommandQueue(context,
            device_id[i],
            NULL,
            &err);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

}

void Knapsack::createProgramBuild(int i) {
    /*A program object in OpenCL encapsulates the program sources or 
     * a binary file*/

    string sourceKernel, line;


    ofs.open("..\\OpenCL\\Knapsack OpenCL\\knapsack_toth.cl", ios_base::in);
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


    program = clCreateProgramWithSource(
            context,
            1, //count pointers
            (const char **) &x,
            NULL,
            &err);

    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clBuildProgram(
            program,
            1,
            &device_id[i], // If device_id is NULL value, the program executable is built for all devices associated with program for which a source or binary has been loaded
            NULL, //options
            NULL, //callback function
            NULL);

    if (err != CL_SUCCESS) {
        cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
        printf("Error building program\n");
        char buffer[4096];
        size_t length;
        clGetProgramBuildInfo(
                program,
                device_id[i],
                CL_PROGRAM_BUILD_LOG,
                sizeof (buffer),
                buffer,
                &length
                );
        printf("%s\n", buffer);
        exit(1);
    }


}

void Knapsack::createMemObjects() {
    /*
     The OpenCL framework provides a way to package data into a memory object. 
     * By using a memory object, OpenCL allows easy packaging of all data and 
     * easy transfer to the compute device memory so that the kernel function 
     * executing on the device has local access to the data. Using a memory 
     * object minimizes memory transfers from the host and device as the kernel 
     * processes data.
     */

    //http://stackoverflow.com/questions/3832963/two-ways-to-create-a-buffer-object-in-opencl-clcreatebuffer-vs-clcreatebuffer
    f0_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify;  the usage of memory
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    f1_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    m_d_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify
            sizeof (int)*capacity, // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*Enqueues a command to write data from host to memory to a buffer object.
     *This functional is typically used to provide data for the kernel processing. 
     */

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            f0_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f0, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            f1_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f1, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            m_d_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*capacity, //size in bytes of data to write to
            m_d, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
}

void Knapsack::createKernel(cl_mem f_input, cl_mem f_output, int weightk, int valuek, int i) {

    /*A kernel object is an encapsulation of the specify
     * __kernel function along with the arguments that are associated with the 
     * __kernel function when executed. The kernel object is eventually sent to 
     * the command queue for execution.*/

    kernel = clCreateKernel(
            program, // a valid program object that has been successfully built
            "knapsack", // the name of the kernel declared with __kernel
            &err // error return code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            0, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &f_input //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            1, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &f_output//a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            2, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &m_d_mem //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            3, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &cmax //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            4, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &weightk //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            5, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &valuek //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*each kernel execution in OpenCL is called a work-item. 
     * OpenCL exploits parallel computation of the compute devices by having 
     * instances of the kernel execute on different portions of the 
     * N-dimensional problem space. In addition, each work-item is executed 
     * only with its assigned data. Thus, it is important specify to OpenCL 
     * how many work-items are needed to process all data.*/
    err = clGetKernelWorkGroupInfo(kernel,
            device_id[i],
            CL_KERNEL_WORK_GROUP_SIZE,
            sizeof (size_t),
            &size,
            NULL);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    cout << "\nCL_KERNEL_WORK_GROUP_SIZE: " << size << endl;

}

void Knapsack::executeMemObjects(cl_mem f_output_mem, int *f) {
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
            NULL // event object to return on completion
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /* Enqueues a command to read data from a buffer object into host memory.
     * This function is typically used to READ RESULTS FROM the KERNEL execution
     * back to host program. 
     * Reads the cl memory "output_mem" on device to the host variable "f".
     */


    err = clEnqueueReadBuffer(
            queue, //valid command queue 
            f_output_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueReadBuffer(
            queue, //valid command queue 
            m_d_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*numelem, //size in bytes of data to write to
            m_d, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "m_d after buffer enqueue: " << endl;
    for (int i = 0; i < capacity; i++)
        cout << m_d[i];

    cout << endl;



}

void Knapsack::printResults() {
    cout << "\nPRINTOUT OF THE RESULTS: " << endl;
    cout << "*************************************************" << endl;
    cout << "Evolution of the Knapsack worth F[x]: ";
    for (int i = 0; i < capacity + 1; i++) {
        if (numelem % 2 == 0) {
            cout << f0[i] << " ";
        } else {
            cout << *(f1 + i) << " ";
        }

    }
    cout << endl;
    cout << "Matrix of decisions M[items][capacity]: " << endl;
    for (int i = 0; i < numelem * capacity; i++) {
        cout << *(M + i) << "; ";
        if ((i + 1) % (capacity) == 0)cout << endl;
    }
    cout << "\nKnapsack's worth: ";
    if (numelem % 2 == 0) {
        cout << f0[capacity] << endl;
    } else {
        cout << f1[capacity] << endl;
    }

    cout << "Chosen items are:" << endl;
    int cap = capacity;
    for (int item = numelem - 1; item > -1; item--) {
        for (int c = cap - 1; c > -1; c--) {
            if (M[item * (capacity) + c] != 0) {
                cout << "Value: " << value[item];
                cout << "\tWeight: " << weight[item] << endl;
                cap = cap - weight[item];
                break;
            } else if (M[item * (capacity) + c] == 0) {
                c = 0;
            }
        }

    }
    for (int i = 0; i < capacity + 1; i++) {
        *(f1 + i) = 0; //f[i] = 0;
        *(f0 + i) = 0;
    }
    cout << "*************************************************" << endl;
    cout << "END OF THE PRINTOUT" << endl;
}

size_t Knapsack::getLocalWorkItems(size_t globalThreads, size_t maxWorkItemSize) {
    /* Each kernel execution in OpenCL is called a work-item thus is important 
     * specify to OpenCL how many work-items are needed to process all data.
     * 
     */
    if (maxWorkItemSize < globalThreads) {
        if (globalThreads % maxWorkItemSize == 0) {
            return maxWorkItemSize;
        } else {
            for (size_t i = maxWorkItemSize - 1; i > 0; --i) {
                if (globalThreads % i == 0) {
                    return i;
                }
            }
        }
    } else {
        return globalThreads;
    }
    return SDK_SUCCESS;
}

void Knapsack::cleanup() {
    err = clReleaseKernel(kernel);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    err = clReleaseProgram(program);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clReleaseMemObject(f0_mem);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    err = clReleaseMemObject(f1_mem);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    err = clReleaseCommandQueue(queue);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    err = clReleaseContext(context);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    free(platforms);
    free(device_id);
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
        case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
            return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case CL_PLATFORM_NOT_FOUND_KHR:
            return "CL_PLATFORM_NOT_FOUND_KHR";
        case CL_DEVICE_PARTITION_FAILED_EXT:
            return "CL_DEVICE_PARTITION_FAILED_EXT";
        case CL_INVALID_PARTITION_COUNT_EXT:
            return "CL_INVALID_PARTITION_COUNT_EXT";
        default:
            return "unknown error code";
    }
}

Knapsack::Knapsack() {
    int vtemp[] = {1, 3, 2, 4};
    int wtemp[] = {2, 3, 2, 2};
    capacity = 5;
    numelem = sizeof (vtemp) / sizeof (int);

    value = (int *) malloc(sizeof (int)*numelem);
    weight = (int *) malloc(sizeof (int)*numelem);
    f1 = (int *) malloc(sizeof (int)*(capacity + 1)); //f[capacity +1]
    f0 = (int *) malloc(sizeof (int)*(capacity + 1)); //f[0, ..., capacity]
    M = (int *) malloc(sizeof (int)*numelem * capacity); //M[numelem][capacity]
    m_d = (int *) malloc(sizeof (int)*(capacity)); //m_d[1, ..., capacity] this a row of M matrix
    global_work_items = (size_t *) malloc(sizeof (size_t));
    local_work_items = (size_t *) malloc(sizeof (size_t));

    memcpy(value, vtemp, sizeof (int)*numelem);
    memcpy(weight, wtemp, sizeof (int)*numelem);
    sumWeight = 0;

    for (int i = 0; i < numelem; i++) {
        sumWeight += weight[i];
    }

    for (int i = 0; i < capacity + 1; i++) {
        *(f1 + i) = 0; //f[i] = 0;
        *(f0 + i) = 0;
    }

    for (int i = 0; i < numelem * capacity; i++) {
        *(M + i) = 0; //M[i]=0;

        if (i < capacity) {
            *(m_d + i) = 0;
        }
    }

    int x1[capacity + 1] = {0};
    int x0[capacity + 1] = {0};
    int m1[capacity] = {0};
    memcpy(f1, x1, sizeof (int)*(capacity + 1));
    memcpy(f0, x0, sizeof (int)*(capacity + 1));
    memcpy(m_d, m1, sizeof (int)*(capacity));

}

void Knapsack::executeComputation(int i) {
    int sumK = sumWeight;
    for (int k = 0; k < numelem; k++) {

        sumK = sumK - weight[k];
        cmax = 0;
        cmax = max(capacity - sumK, weight[k]);

        *global_work_items = (size_t) (capacity - cmax + 1);
        *local_work_items = getLocalWorkItems(*global_work_items, *device_max_work_group_size);
        cout << "global_work_items: " << *global_work_items << endl;
        cout << "local_work_items: " << *local_work_items << endl;

        for (int i = 0; i < capacity; i++)
            cout << *(m_d + i);
        cout << endl;

        if (k % 2 == 0) {
            //createKernel(f0_mem, f1_mem, weight[k], value[k], i);
            //executeMemObjects(f1_mem, f1);
            pari(weight[k], value[k], i);
        } else {
            //createKernel(f1_mem, f0_mem, weight[k], value[k], i);
            //executeMemObjects(f0_mem, f0);
            dispari(weight[k], value[k], i);
        }
        memmove(M + k*capacity, m_d, sizeof (int)*capacity);
        cout << k << ": ";
        for (int i = 0; i < capacity; i++) {
            cout << *(m_d + i);
            *(m_d + i) = 0;
        }
        cout << endl;
    }

}

void Knapsack::pari(int weightk, int valuek, int i) {

    f0_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify;  the usage of memory
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    f1_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    m_d_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify
            sizeof (int)*capacity, // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*Enqueues a command to write data from host to memory to a buffer object.
     *This functional is typically used to provide data for the kernel processing. 
     */

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            f0_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f0, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            f1_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f1, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            m_d_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*capacity, //size in bytes of data to write to
            m_d, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*A kernel object is an encapsulation of the specify
     * __kernel function along with the arguments that are associated with the 
     * __kernel function when executed. The kernel object is eventually sent to 
     * the command queue for execution.*/

    kernel = clCreateKernel(
            program, // a valid program object that has been successfully built
            "knapsack", // the name of the kernel declared with __kernel
            &err // error return code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            0, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &f0_mem //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            1, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &f1_mem//a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            2, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &m_d_mem //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            3, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &cmax //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            4, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &weightk //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            5, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &valuek //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*each kernel execution in OpenCL is called a work-item. 
     * OpenCL exploits parallel computation of the compute devices by having 
     * instances of the kernel execute on different portions of the 
     * N-dimensional problem space. In addition, each work-item is executed 
     * only with its assigned data. Thus, it is important specify to OpenCL 
     * how many work-items are needed to process all data.*/
    
    err = clGetKernelWorkGroupInfo(kernel,
            device_id[i],
            CL_KERNEL_WORK_GROUP_SIZE,
            sizeof (size_t),
            &size,
            NULL);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    cout << "\nCL_KERNEL_WORK_GROUP_SIZE: " << size << endl;

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
            NULL // event object to return on completion
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

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
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueReadBuffer(
            queue, //valid command queue 
            f1_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f1, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "f1 after buffer enqueue: " << endl;
    for (int i = 0; i < capacity + 1; i++)
        cout << f1[i];
    cout << endl;
    cout << "m_d after buffer enqueue: " << endl;
    for (int i = 0; i < capacity; i++)
        cout << m_d[i];

    cout << endl;
}

void Knapsack::dispari(int weightk, int valuek, int i) {


    f0_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify;  the usage of memory
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    f1_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify
            sizeof (int)*(capacity + 1), // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    m_d_mem = clCreateBuffer(
            context, // a valid context
            CL_MEM_READ_WRITE, // bit-field flag to specify
            sizeof (int)*capacity, // size in bytes of the buffer to allocated
            NULL, // pointer to buffer data to be copied from host
            &err // returned error code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*Enqueues a command to write data from host to memory to a buffer object.
     *This functional is typically used to provide data for the kernel processing. 
     */

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            f0_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f0, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            f1_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f1, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clEnqueueWriteBuffer(
            queue, //valid command queue 
            m_d_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*capacity, //size in bytes of data to write to
            m_d, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;




    /*A kernel object is an encapsulation of the specify
     * __kernel function along with the arguments that are associated with the 
     * __kernel function when executed. The kernel object is eventually sent to 
     * the command queue for execution.*/

    kernel = clCreateKernel(
            program, // a valid program object that has been successfully built
            "knapsack", // the name of the kernel declared with __kernel
            &err // error return code
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            0, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &f1_mem //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            1, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &f0_mem//a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            2, //the specific argument of a kernel
            sizeof (cl_mem), //the size of the argument data
            &m_d_mem //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            3, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &cmax //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            4, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &weightk //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    err = clSetKernelArg(
            kernel, //valid kernel object
            5, //the specific argument of a kernel
            sizeof (int), //the size of the argument data
            &valuek //a pointer of data used as the argument
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /*each kernel execution in OpenCL is called a work-item. 
     * OpenCL exploits parallel computation of the compute devices by having 
     * instances of the kernel execute on different portions of the 
     * N-dimensional problem space. In addition, each work-item is executed 
     * only with its assigned data. Thus, it is important specify to OpenCL 
     * how many work-items are needed to process all data.*/
    
    err = clGetKernelWorkGroupInfo(kernel,
            device_id[i],
            CL_KERNEL_WORK_GROUP_SIZE,
            sizeof (size_t),
            &size,
            NULL);
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;
    cout << "\nCL_KERNEL_WORK_GROUP_SIZE: " << size << endl;

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
            NULL // event object to return on completion
            );
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    /* Enqueues a command to read data from a buffer object into host memory.
     * This function is typically used to READ RESULTS FROM the KERNEL execution
     * back to host program. 
     * Reads the cl memory "output_mem" on device to the host variable "f".
     */


    err = clEnqueueReadBuffer(
            queue, //valid command queue 
            f0_mem, //memory buffer object to write to
            CL_TRUE, // indicate blocking write
            0, //the offset in the buffer object to write to
            sizeof (int)*(capacity + 1), //size in bytes of data to write to
            f0, //pointer to buffer in host mem to read data from
            0, //number of events in the event list 
            NULL, //list of events that needs to complete before this executes
            NULL); //event object to return on completion
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

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
    if (err != CL_SUCCESS)cout << "\n!!!" << Knapsack::getErrorCode(err) << endl;

    cout << "f0 after buffer enqueue: " << endl;
    for (int i = 0; i < capacity + 1; i++)
        cout << f0[i];
    cout << endl;
    cout << "m_d after buffer enqueue: " << endl;
    for (int i = 0; i < capacity; i++)
        cout << m_d[i];

    cout << endl;
}

int Knapsack::getNumDevices() {
    return (int) num_devices;
}
