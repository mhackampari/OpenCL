/*
 * File:   knapsack_main.cpp
 * Author: terminator
 *
 * Created on February 21, 2015, 10:07 PM
 */
#include "Knapsack.h"
using namespace std;
/*query the total number of platforms available in the system.
  There can be more than one platform.*/
void Knapsack::queryOCLdevices() {

    
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
            
            //prepare to allocate of the available platforms
            platform_info = (char *) malloc(size * sizeof (char));

            err = clGetPlatformInfo(
                    platforms[i],
                    platform_const_value[j],
                    size,
                    platform_info,
                    NULL);

            cout << platform_const_string[j] << ": " << string(platform_info) << "\n";
        }
    }

}

int main(int argc, char** argv) {

    //1.Query the host system for OpenCL devices.
    Knapsack ksack;
    ksack.queryOCLdevices();
    return 0;
}

