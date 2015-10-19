/* 
 * File:   TestData.cpp
 * Author: terminator
 * 
 * Created on April 21, 2015, 2:40 PM
 */
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <sys/sysinfo.h>
#endif
#include "TestData.h"

TestData::TestData() {

    numelem = 1000;
    weight.resize(numelem, 0);
    value.resize(numelem, 0);
    sum = 0;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937_64 generator(seed);
    for (int i = 0; i < numelem; i++) {

        weight[i] = generator() % numelem + 1;
        sum += weight[i];
        value[i] = weight[i] + 50;

    }

    capacity = sum / 2;

    //picked this interval without any underlying criteria
    // unifdist(0,1000);
    default_random_engine random_engine;
    uniform_int_distribution<int> unifdist(1, numelem);

    for (int i = 0; i < numelem; i++) {

        weight[i] = unifdist(random_engine);
        sum += weight[i];
        value[i] = weight[i] + 50;

    }

    capacity = sum / 2;

}

TestData::TestData(int numel, bool verbose) {

    numelem = numel;
    weight.resize(numelem, 0);
    value.resize(numelem, 0);
    sum = 0;

    default_random_engine random_engine;
    uniform_int_distribution<int> unifdist(1, numelem);

    for (int i = 0; i < numelem; i++) {

        weight[i] = rand() % 100 + 1; //unifdist(random_engine);
        sum += weight[i];
        value[i] = weight[i] + 50; //+ 50;
        if (verbose)
            cout << i << "weight: " << weight[i] << "\t" << "value: " << value[i] << endl;

    }
    capacity = sum / 2;
    if (verbose||true)
           cout << "Capacity: " << capacity << endl;

    unsigned int ram_capacity = 0;

#ifdef __linux__
    cout << "__LINUX__ MACRO\n";
    struct sysinfo info;
    if (sysinfo(&info) != 0)
        cout << "sysinfo error";
    cout << "INFO FREE RAM: " << (float)info.freeram/(1024*1024*1024) << "GB\n";
    cout << "INFO AVAILABLE RAM: " << (float)info.totalram/(1024*1024*1024) << "GB\n";

    //we cannot allocate the whole ram, just a % of it
    ram_capacity = info.freeram * 0.8;
    ram_capacity = info.totalram;

#endif

#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);
    cout << "\n" << statex.ullAvailPhys << " \n\n";

    //we cannot allocate the whole ram, just a % of it
    ram_capacity = statex.ullAvailPhys / (sizeof (unsigned int) * 1024 + numel/10);
    cout << "Capacity: " << capacity << endl;
#endif

    //min ram: capacity * numelem/32 * 4 bytes
    if (capacity*numel/32*sizeof(unsigned int) < ram_capacity){
        capacity = sum / 2;
        cout << "RAM ok! ...Loading... \n";
    }
    else{
        capacity = ram_capacity/(numel/32*sizeof(unsigned int));
        cout << "RAM IS NOT SUFFICENT: Reducing capacity from: " 
        << (sum / 2) <<" to: " << capacity;
    }


    }

TestData::~TestData() {
    //cout << "\nTEST DATA HAVE BEEN JUST DESTROYED\n";
}

int* TestData::getValue() {

    return value.data();
}

int* TestData::getWeight() {

    return weight.data();
}

int TestData::getSum() {

    return sum;
}

int TestData::getCapacity() {

    return capacity;
}

int TestData::getNumelem() {
    return numelem;
}


