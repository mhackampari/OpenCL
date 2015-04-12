/*
 * File:   knapsack_main.cpp
 * Author: terminator
 *TODO: write to the file the results and add timer function
 * Created on February 21, 2015, 10:07 PM
 */
#include <CL/cl.h>
#include "Knapsack.h"

int main(int argc, char** argv) {

    Knapsack ksack;
    fstream logfile;
    logfile.open("results_knapsack.txt", ios::out);
    int e = 2;
    int b = 1;
    int clk = 4;
    b-=e-clk;
    cout<< b <<endl;
    
    //1.Query the host system for OpenCL platform info
    ksack.queryOclPlatformInfo(&logfile);
    //2. Query the host for OpenCL devices
    ksack.queryOclDevice(&logfile);

    for (int i=0; i < ksack.getNumDevices(); i++) {

        ksack.queryOclDeviceInfo(i, &logfile);
        //3. Create a context and a queue to associate the OpenCL devices.
        ksack.createContextQueue(i, &logfile);
        //4. Create programs that will run on one or more associated devices.
        ksack.createProgramBuild(i, &logfile);
        //5. Create memory objects on the host or on the device.
        //6. Copy memory data to the device as needed. Create memory objects on the host or on the device.
        ksack.createMemObjects(&logfile);
        //7. Create Kernels
        //8. Provide arguments for the kernels.
        //ksack.createKernel();
        //9. Submit the kernels to the command queue for execution.
        //10. Copy the results from the device to the host
        //ksack.executeMemObjects();
        
        ksack.executeComputation(i, &logfile);

        ksack.printResults(&logfile);


    }
    logfile.close();
    ksack.cleanup();
    return 0;
}

