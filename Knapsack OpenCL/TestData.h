/* 
 * File:   TestData.h
 * Author: terminator
 *
 * Created on April 21, 2015, 2:40 PM
 */

#ifndef TESTDATA_H
#define	TESTDATA_H

#include <vector>
#include <iostream>
#include <random>

using namespace std;

class TestData {
public:
    TestData();
    TestData(int);
    virtual ~TestData();
    void init_data();
    int* getValue();
    int* getWeight();
    int getSum();
    int getCapacity();
    int getNumelem();
    const int x=0;
private:
    vector<int> weight;
    vector<int> value;
    int sum;
    int capacity;
    int numelem;
    default_random_engine random_engine;
    //uniform_int_distribution<int> unifdist(int,int);
    //uniform_int_distribution<int> unifdist(default_random_engine);
   
};

#endif	/* TESTDATA_H */

