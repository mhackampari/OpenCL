/* 
 * File:   TestData.cpp
 * Author: terminator
 * 
 * Created on April 21, 2015, 2:40 PM
 */
#ifndef TESTDATA_H
#define	TESTDATA_H

#include <vector>
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>


using namespace std;

class TestData {
public:
    TestData();
    TestData(int numelm, bool verbose);
    TestData(const TestData& t);
    virtual ~TestData();
    int* getValue();
    int* getWeight();
    int getSum();
    int getCapacity();
    int getNumelem();
    
private:
    vector<int> weight;
    vector<int> value;
    int sum;
    int capacity;
    int numelem;
   
};

#endif	/* TESTDATA_H */

