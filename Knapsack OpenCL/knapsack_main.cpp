/*
 * File:   knapsack_main.cpp
 * Author: terminator
 *TODO: write to the file the results and add timer function
 * Created on February 21, 2015, 10:07 PM
 */
#include <CL/cl.h>
#include "Knapsack.h"
#include "Chrono.h"
#include "Timer.h"
#include "TestData.h"

int main(int argc, char** argv) {
    Chrono ch;
    Timer tm;
    tm.start();
    ch.startChrono();
    fstream logfile;
    logfile.open("results_knapsack.txt", ios::out);
    //remember the limit of the 4gb allocated space
    int dim[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int local_threads[] = {0, 64, 128, 256, 512, 1024};


    for (int k = 5; k < sizeof (local_threads) / sizeof (int); k++) {//local_threads
        for (int j = 9; j < sizeof (dim) / sizeof (int); j++) {

            TestData* test = new TestData(dim[j]);
            Knapsack *ksack = new Knapsack(*test);

            //1.Query the host system for OpenCL platform info
            ksack->queryOclPlatformInfo(&logfile);
            //2. Query the host for OpenCL devices
            ksack->queryOclDevice(&logfile);

            for (int i = 0; i < ksack->getNumDevices(); i++) {

                ksack->queryOclDeviceInfo(i, &logfile);
                //3. Create a context and a queue to associate the OpenCL devices.
                ksack->createContextQueue(i, &logfile);
                //4. Create programs that will run on one or more associated devices.
                ksack->createProgramBuild(i, &logfile);
                //5. Create memory objects on the host or on the device.
                //6. Copy memory data to the device as needed. Create memory objects on the host or on the device.
                ksack->createMemObjects(&logfile);
                //7. Create Kernels
                ksack->createKernel();
                //8. Provide arguments for the kernels.
                // ksack.createKernel();
                //9. Submit the kernels to the command queue for execution.
                //10. Copy the results from the device to the host
                //ksack.executeMemObjects();
                
                ksack->executeComputation(i, &logfile, local_threads[k]);
                ksack->printResults(&logfile);

            }
            delete ksack;
            delete test;

        }
    }
    logfile.close();
    //http://stackoverflow.com/questions/14063791/double-free-or-corruption-but-why

    ch.stopChrono();
    tm.stop();
    cout << "CHRONO TIME: " << ch.getTimeChrono() << endl;
    cout << "TIMER TIME: " << tm.getTime();

    return 0;

}
