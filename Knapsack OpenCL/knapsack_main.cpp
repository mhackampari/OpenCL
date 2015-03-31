/*
 * File:   knapsack_main.cpp
 * Author: terminator
 *
 * Created on February 21, 2015, 10:07 PM
 */
#include <CL/cl.h>
#include "Knapsack.h"

int main(int argc, char** argv) {

    Knapsack ksack;
    //1.Query the host system for OpenCL platform info
    ksack.queryOclPlatformInfo();
    //2. Query the host for OpenCL devices
    ksack.queryOclDevice();

    for (int i=0; i < ksack.getNumDevices(); i++) {

        ksack.queryOclDeviceInfo(i);
        //3. Create a context and a queue to associate the OpenCL devices.
        ksack.createContextQueue(i);
        //4. Create programs that will run on one or more associated devices.
        ksack.createProgramBuild(i);
        //5. Create memory objects on the host or on the device.
        //6. Copy memory data to the device as needed. Create memory objects on the host or on the device.
        ksack.createMemObjects();
        //7. Create Kernels
        //8. Provide arguments for the kernels.
        //ksack.createKernel(i);
        //9. Submit the kernels to the command queue for execution.
        //10. Copy the results from the device to the host
        //ksack.executeMemObjects();
        ksack.executeComputation(i);

        ksack.printResults();


    }
    ksack.cleanup();
    return 0;
}

