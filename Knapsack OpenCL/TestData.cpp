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
    default_random_engine random_engine;
    uniform_int_distribution<int> unifdist(1,numelem);
    
    for (int i = 0; i < numelem; i++) {
        weight[i] = unifdist(random_engine); 
        sum += weight[i];
        value[i] = weight[i] + 50;
    }

    capacity = sum / 2;

}

TestData::TestData(int numel) {

    numelem = numel;
    weight.resize(numelem, 0);
    value.resize(numelem, 0);
    sum = 0;
    
    default_random_engine random_engine;
    uniform_int_distribution<int> unifdist(1,numelem);
    
    for (int i = 0; i < numelem; i++) {
        weight[i] = i+1;//rand()%numelem + 1;//unifdist(random_engine); 
        sum += weight[i];
        value[i] = weight[i]+rand()%numelem + 1; //+ 50;
        
        cout << "weight[i]" << weight[i] <<"\t";
        cout << "value[i]" << value[i] <<"\n";
    }

    capacity = sum / 2;
    cout << "Capacity: " << capacity << endl; 
    

}

TestData::TestData(const TestData& t) {

    cout << "COPY CONSTRUCTOR\n";
    
    numelem = this->numelem;
    weight.resize(numelem, 0);
    value.resize(numelem, 0);
    sum = this->sum;
    weight = this->weight;
    value = this->value;
    capacity = this->capacity;
    

}

TestData::~TestData() {
    cout << "\nTEST DATA HAVE BEEN JUST DESTROYED\n";
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


