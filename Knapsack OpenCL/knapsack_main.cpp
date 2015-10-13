/*
 * File:   knapsack_main.cpp
 * Author: terminator
 * 
 * This work is based on this article:
 * Solving knapsack problems on GPU by V. Boyera, D. El Baza,  M. Elkihel
 * https://cel.archives-ouvertes.fr/hal-01152223/document
 * related work: Accelerating the knapsack problem on GPUs by Bharath Suri
 * http://www.diva-portal.org/smash/get/diva2:439054/FULLTEXT01.pdf
 * Created on February 21, 2015, 10:07 PM
 */
#include <CL/cl.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>

#define NUMELEM 240
using namespace std;

class Knapsack {
    const string kernelString =
            "void kernel knapsack(global int *input_f, global int *output_f, global uint *m_d,  int cmax, int weightk, int pk, int maxelem, int i){\
                int c = get_global_id(0)+cmax;\
                if(get_global_id(0)<maxelem){\
                    if(input_f[c] < input_f[c - weightk] + pk){\
                        output_f[c] = input_f[c - weightk] + pk;\
                        m_d[c-1] += pown(2.0,i);\
                    }\
                    else{\
                    output_f[c] = input_f[c];\
                    }\
                }\
            }";
private:
    int capacity;
    int sum;
    const int numelem = NUMELEM;
    cl_int err;
    vector<int> weight;
    vector<int> value;
    cl_command_queue queue;
    cl_context context;
    cl_program program;
    cl_kernel kernel, kernel1, kernel2;
    cl_mem in;
    cl_ulong run_time;

    vector<unsigned int> m_d;
    vector<unsigned int> M; //allocate dynamically in execute phase
    cl_uint num;

    vector<cl_platform_id> platforms;
    vector<cl_device_id> device_id;
public:
    cl_mem in_even_mem, in_odd_mem, out_even_mem, out_odd_mem, m_d_mem, f0_mem, f1_mem;
    vector<int> f0;
    vector<int> f1;


public:

    Knapsack() {
        generateRandomData();
        f0.resize(capacity + 1, 0);
        f1.resize(capacity + 1, 0);
        m_d.resize(capacity, 0);
        err = 0;
        num = 1; //Interested in only one platform
        platforms.resize(num);
    }

    void generateRandomData() {
        sum = 0;
        value.resize(numelem, 0);
        weight.resize(numelem, 0);
        default_random_engine random_engine;
        uniform_int_distribution<int> unifdist(1, numelem);

        for (int i = 0; i < numelem; i++) {
            weight[i] = rand()%numelem+1;//unifdist(random_engine);
            sum += weight[i];
            value[i] = weight[i] + 50;
        }

        capacity = sum / 50;
        //capacity = CAPACITY;
        cout << "Capacity: " << capacity << endl;

        vector<int>::iterator it_weight, it_value;
        it_weight = weight.begin();
        it_value = value.begin();
        for (int i = 0; i < numelem; i++) {
            cout << i << "weight: " <<  *(it_weight + i) << "\t" << "value: " << *(it_value + i)<< endl;
        }

    }

    void initDevices() {
        err = clGetPlatformIDs(num, platforms.data(), NULL);
        checkError(err);
        err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, 0, &num);
        checkError(err);
        device_id.resize(num);
        err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, num, device_id.data(), NULL);
        checkError(err);
    }

    void createContextAndQueue(int i) {
        const cl_context_properties context_properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties) platforms[0], 0};
        context = clCreateContextFromType(context_properties, CL_DEVICE_TYPE_ALL, 0, 0, &err);
        checkError(err);
        queue = clCreateCommandQueue(context, device_id[i], CL_QUEUE_PROFILING_ENABLE, &err);
        checkError(err);
    }

    void createProgramAndBuild(int i) {

        program = clCreateProgramWithSource(context, 1, (const char **) &kernelString, NULL, &err);
        checkError(err);
        err = clBuildProgram(program, 1, &device_id[i], NULL, NULL, NULL);
        checkError(err);

        if (err != CL_SUCCESS) {
            char buffer[4096] = {""};
            size_t length = 4096;
            clGetProgramBuildInfo(program, device_id[i], CL_PROGRAM_BUILD_LOG, 4096 * sizeof (buffer), buffer, 0);
            cout << string(buffer) << "\n";

            exit(1);
        }

    }

    void createMemoryObjects() {

        f1_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof (unsigned int)*(capacity + 1), NULL, &err);
        checkError(err);
        f0_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof (unsigned int)*(capacity + 1), NULL, &err);
        checkError(err);
        m_d_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof (unsigned int)*(capacity), NULL, &err);
        checkError(err);

    }

    void createKernel() {
        kernel = clCreateKernel(program, "knapsack", &err);
        checkError(err);
        kernel1 = clCreateKernel(program, "knapsack", &err);
        checkError(err);
        kernel2 = clCreateKernel(program, "knapsack", &err);
        checkError(err);
    }

    void writeBufferToDevice(cl_mem& in, cl_mem& out, int* input, int* output) {

        err = clEnqueueWriteBuffer(queue, in, CL_TRUE, 0, sizeof (unsigned int)*(capacity + 1), input, 0, NULL, NULL);
        checkError(err);
        err = clEnqueueWriteBuffer(queue, out, CL_TRUE, 0, sizeof (unsigned int)*(capacity + 1), output, 0, NULL, NULL);
        checkError(err);
        err = clEnqueueWriteBuffer(queue, m_d_mem, CL_TRUE, 0, sizeof (unsigned int)*(capacity), m_d.data(), 0, NULL, NULL);
        checkError(err);

    }

    void writeBuffer_m_d_ToDevice() {

        err = clEnqueueWriteBuffer(queue, m_d_mem, CL_TRUE, 0, sizeof (unsigned int)*(capacity), m_d.data(), 0, NULL, NULL);
        checkError(err);

    }

    void readBufferFromDevice(cl_mem &output_mem, int* output) {

        err = clEnqueueReadBuffer(queue, m_d_mem, CL_TRUE, 0, sizeof (unsigned int)*capacity, m_d.data(), 0, NULL, NULL);
        checkError(err);

        err = clEnqueueReadBuffer(queue, output_mem, CL_TRUE, 0, sizeof (unsigned int)*(capacity + 1), output, 0, NULL, NULL);
        checkError(err);
    }

    void readBuffer_m_d_FromDevice() {

        err = clEnqueueReadBuffer(queue, m_d_mem, CL_TRUE, 0, sizeof (unsigned int)*capacity, m_d.data(), 0, NULL, NULL);
        checkError(err);

        //err = clEnqueueReadBuffer(queue, output_mem, CL_TRUE, 0, sizeof (int)*(capacity + 1), output, 0, NULL, NULL);
        //checkError(err);
    }

    void setKernelArgs(cl_mem& input, cl_mem& output, int cmax, int total_elements, int k, int i) {

        cl_int err = clSetKernelArg(kernel, 0, sizeof (cl_mem), &input);
        checkError(err);

        err = clSetKernelArg(kernel, 1, sizeof (cl_mem), &output);
        checkError(err);

        err = clSetKernelArg(kernel, 2, sizeof (cl_mem), &m_d_mem);
        checkError(err);

        err = clSetKernelArg(kernel, 3, sizeof (int), &cmax);
        checkError(err);

        err = clSetKernelArg(kernel, 4, sizeof (int), &weight.at(k));
        checkError(err);

        err = clSetKernelArg(kernel, 5, sizeof (int), &value.at(k));
        checkError(err);

        err = clSetKernelArg(kernel, 6, sizeof (int), &total_elements);
        checkError(err);

        err = clSetKernelArg(kernel, 7, sizeof (int), &i);
        checkError(err);

    }

    void executeNDRange(int total_elements, int i) {

        cl_event prof_event = 0;
        size_t size;
        int device_max_group_size = 0;
        cl_int err = clGetDeviceInfo(device_id[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, 0, 0, &size);
        checkError(err);
        err = clGetDeviceInfo(device_id[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, size, &device_max_group_size, NULL);
        checkError(err);

        size_t local_threads = getLocalWorkItems(total_elements, device_max_group_size);
        size_t global_threads = getGlobalWorkItems(total_elements, local_threads);

        err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_threads, &local_threads, 0, NULL, &prof_event);
        checkError(err);

        err = clFinish(queue);
        checkError(err);

        cl_ulong start_time = 0;
        cl_ulong end_time = 0;

        err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_QUEUED, sizeof (cl_ulong), &start_time, NULL);
        checkError(err);

        err = clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end_time, NULL);
        checkError(err);

        run_time += (end_time - start_time) * pow(10, -6);


    }

    void printResults() {
        cout << "\nPRINTOUT OF THE RESULTS: " << endl;
        cout << "*************************************************" << endl << endl;
        cout << "Matrix of decisions M[items][capacity]: " << endl;
/*
        for (int i = 0; i < ceil(numelem / 32.0) * capacity; i++) {

            unsigned int x = ceil(log2(M[i])); //x is the position 0...31
            cout << i << ": # of bits: " << x << "\t M[]: " << M[i] << "\n";
            if ((i + 1) % (capacity) == 0) {
                cout << "***" << endl;
            }
        }
*/
        int c = capacity;
        unsigned int bit = pow(2, 31);
        int bit32 = 32;
        int worth = 0;
        int capacita = 0;

        for (int i = ceil(numelem / 32.0) - 1; i >= 0; i--) {
            bit32 = 32;
            cout << "**numelem / 32.0**" << endl;
            while (bit32 > 0 && c > 0) {

                unsigned int m = M[i * capacity + (c - 1)];
                unsigned int x = ceil(log2(M[i * capacity + (c - 1)])); // gives the position of the msb: 000100 = 2

                unsigned int bit32pw = pow(2, (bit32 - 1));
                if (bit32pw == (bit32pw & m)) { //binary: 1000 == 1000 <- (1000 & 1010)
                    cout << "M" << i * capacity + (c - 1) << ": " << M[i * capacity + (c - 1)] << endl;
                    cout << x << "; ";
                    cout << i * 32 + bit32 - 1 << " \tvalue: " << value[i * 32 + bit32 - 1] << "\tweight: " << weight[i * 32 + bit32 - 1] << endl;
                    c -= weight[i * 32 + bit32 - 1];
                    capacita += weight[i * 32 + bit32 - 1];
                    worth += value[i * 32 + bit32 - 1];
                }
                bit32--;

            }
        }

        cout << "Chosen items are:" << endl;

        int cap = capacity;

        cout << "SumWeight: " << sum << "\t Capacity: " << capacity << endl;
        cout << "Knapsack's worth: " << worth << "\t Weight of the selected objects: " << capacita << endl;
        cout << "run_time: " << run_time / numelem * 1.0 << endl;
        run_time = 0;

        //http://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0
        fill(f0.begin(), f0.end(), 0); // f0.assign(f0.size(),0);
        fill(f1.begin(), f1.end(), 0); // f1.assign(f1.size(),0);

        cout << "*************************************************" << endl;
        cout << "END OF THE PRINTOUT" << endl;

    }

    void cleanUp() {
        checkError(clReleaseMemObject(m_d_mem));
        checkError(clReleaseMemObject(f0_mem));
        checkError(clReleaseMemObject(f1_mem));
        checkError(clReleaseProgram(program));
        checkError(clReleaseContext(context));
        checkError(clReleaseCommandQueue(queue));
    }

    int getLocalWorkItems(int totalThreads, int localThreads) {

        if (localThreads < totalThreads) {

            return localThreads;

        } else {

            return totalThreads;

        }

    }

    int getGlobalWorkItems(int globalThreads, int localThreads) {

        if (localThreads < globalThreads && globalThreads % localThreads != 0) {

            //Workgroup should be full of threads to have max throughput
            globalThreads = (globalThreads / localThreads + 1) * localThreads;

        }
        return globalThreads;
    }

    void checkError(cl_int e) {
        if (err != CL_SUCCESS) {
            cout << "\n!!!" << getErrorCode(err) << endl;
        }
    }

    string getErrorCode(cl_int e) {

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
    }

    int getWeight(int j) {
        return weight.at(j);
    }

    int getNumelem() {
        return numelem;
    }

    int getCapacity() {
        return capacity;
    }

    int getDeviceSize() {
        return device_id.size();
    }

    int* getf0Ptr() {
        return f0.data();
    }

    int* getf1Ptr() {
        return f1.data();
    }

    int getSum() {
        return sum;
    }

    cl_command_queue getQueue() {
        return queue;
    }

    void writeToM(int j) {
        //segmentation fault can be caused because of max memory limit(ram)
        //memcpy(M + k*capacity, m_d, sizeof (int)*capacity);
        try {
            M.resize(capacity * j, 0);
        } catch (int e) {
            cerr << "BAD_ALLOC CAUGHT: try with smaller 'numelem' number" << e << endl;
            exit(1);
        }



        vector<unsigned int>::iterator it = M.begin()+(j - 1) * capacity;
        M.insert(it, m_d.begin(), m_d.end());

        for (int i = 0; i < capacity; i++) {
            if (m_d[i]  != M[i + (j - 1) * capacity]) {
                cout << "\n!!!!!!!!!!!!!!!!!!!!!m_d<0" << i << " " << m_d[i] << "  M: " << M[i + (j - 1) * capacity] << endl;
                system("pause");
            }

            //cout << "m_d" << i << " " << m_d[i] << "\tM" << i + (j - 1) * capacity << ": " << M[i + (j - 1) * capacity] << endl;
        }

        // http://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0
        m_d.assign(capacity, 0);

    }

    vector<unsigned int>::iterator getMd() {
        return m_d.begin();
    }

};

int main(int argc, char** argv) {

    Knapsack ksack;
    ksack.initDevices();

    for (int i = 0; i < ksack.getDeviceSize(); i++) {

        ksack.createContextAndQueue(i);
        ksack.createProgramAndBuild(i);
        ksack.createKernel();
        ksack.createMemoryObjects();

        int sumK = 0;

        int k_M = 1;
        int total_elements = 0;
        int cmax = 0;
        int capacity = ksack.getCapacity();
        sumK = ksack.getSum();
        int numelem = ksack.getNumelem();

        ksack.writeBufferToDevice(ksack.f0_mem, ksack.f1_mem, ksack.getf0Ptr(), ksack.getf1Ptr());
        for (int k = 0; k < numelem; k++) {

            sumK = sumK - ksack.getWeight(k);
            cmax = max(capacity - sumK, ksack.getWeight(k));
            total_elements = capacity - cmax + 1;
            if (total_elements > 0) { //in case one element exceeds the whole capacity

                //SWAPPING TWO BUFFERS
                if (k % 2 == 0) {
                    ksack.setKernelArgs(ksack.f0_mem, ksack.f1_mem, cmax, total_elements, k, k % 32);
                } else {
                    ksack.setKernelArgs(ksack.f1_mem, ksack.f0_mem, cmax, total_elements, k, k % 32);
                }
                ksack.executeNDRange(total_elements, i);
            }

            if (k % 32 == 31) {
                ksack.readBuffer_m_d_FromDevice();
                //memcpy(M + k*capacity, m_d, sizeof (int)*capacity);
                ksack.writeToM(k_M);
                ksack.writeBuffer_m_d_ToDevice(); //resets m_d_mem 

                k_M += 1;
            }

        }

        if (numelem % 32 != 0) {
            ksack.readBuffer_m_d_FromDevice();
            //memcpy(M + k*capacity, m_d, sizeof (int)*capacity);
            ksack.writeToM(k_M);
        }


        ksack.printResults();
        system("pause");
    }//for device

    ksack.cleanUp();
    return 0;
}

//http://stackoverflow.com/questions/3857981/rationalizing-what-is-going-on-in-my-simple-opencl-kernel-in-regards-to-global-m/3858174#3858174
