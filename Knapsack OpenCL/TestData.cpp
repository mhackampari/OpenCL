/* 
 * File:   TestData.cpp
 * Author: terminator
 * 
 * Created on April 21, 2015, 2:40 PM
 */

#include "TestData.h"

TestData::TestData() {
    numelem = 1000;
    weight.resize(numelem, 0);
    value.resize(numelem, 0);
    sum = 0;
    //picked this interval without any underlying criteria
    // unifdist(0,1000);
    
    init_data();

}

TestData::TestData(int numel) {

    numelem = numel;
    weight.resize(numelem, 0);
    value.resize(numelem, 0);
    sum = 0;
    init_data();

}

TestData::~TestData() {
    cout << "\nTEST DATA HAVE BEEN JUST DESTROYED\n";
}

void TestData::init_data() {
    
    default_random_engine random_engine;
    uniform_int_distribution<int> unifdist(0,1000);
    
    for (int i = 0; i < numelem; i++) {
        weight[i] = unifdist(random_engine); 
        sum += weight[i];
        value[i] = weight[i] + 50;
    }

    capacity = sum / 2;

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
