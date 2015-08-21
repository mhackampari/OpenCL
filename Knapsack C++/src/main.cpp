/* 
 * File:   main.cpp
 * Author: terminator
 *
 * Created on July 22, 2015, 7:08 PM
 */

#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <ctime>
#include <fstream>
#include "Chrono.h"
#include "TestData.h"

using namespace std;
#ifndef PRINT
#define PRINT false
#endif

#ifndef VERBOSE
#define VERBOSE false
#endif

#ifndef NCYCLES
#define NCYCLES 100
#endif

void printResults(int capacity, int sumWeight, int numelem, int* f, int* M,
	int* weight, int* value, long runtime);

void init(int &capacity, int &sumWeight, int numelem, int **f, int **M,
	int **weight, int **value)
    {
    TestData t(numelem, VERBOSE);
    sumWeight = t.getSum();
    capacity = t.getCapacity();
    *value = (int *) calloc(numelem, sizeof(int));
    *weight = (int *) calloc(numelem, sizeof(int));
    memcpy(*value, t.getValue(), sizeof(int) * numelem);
    memcpy(*weight, t.getWeight(), sizeof(int) * numelem);
    *f = (int *) calloc((capacity + 1), sizeof(int)); //f[0, ..., capacity]
    //segmentation fault can be caused because of max memory limit(ram)
    *M = (int *) calloc(numelem * (capacity + 1), sizeof(int)); //M[numelem][capacity]
    if (M == NULL)
	cout << "NOT ENOUGH AVAILABLE SPACE!!!! TRY WITH LESS ELEMENTS\n";
#if VERBOSE
    int sum = 0;
    cout << "Init:\n";
    for (int i = 0; i < numelem; i++)
	{
	cout << i << "weight: " << *(*weight+i) << "\t" << "value: " << *(*value +i) << endl;
	sum += *(*weight+i);
	}
    cout << "SUM WEIGHT AFTER INIT: " << sum << endl;
#endif
    }

int main(int argc, char** argv)
    {

    fstream out_file("knapsack_results.txt", ios::out);
    int numelem[] =
	{
	5, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000
	};
    unsigned int nelements = sizeof(numelem) / sizeof(int);
    Chrono chrono, echrono;
    double time_omp;
    unsigned long time_chrono;

    echrono.startChrono();
    for (unsigned int i = 0; i < nelements; i++)
	{
	time_chrono = 0;
	time_omp = 0;
	for (unsigned int j = 0; j < NCYCLES; j++)
	    {

	    int sumWeight = 100;
	    int capacity = sumWeight / 2;
	    int *value, *weight, *f, *M;

	    init(capacity, sumWeight, numelem[i], &f, &M, &weight, &value);

	    int sumK = sumWeight;

	    chrono.startChrono();
	    for (int k = 0; k < numelem[i]; k++)
		{

		sumK = sumK - weight[k];
		int cmax = 0;
		cmax = max(capacity - sumK, weight[k]);

		for (int c = capacity; c >= cmax; c--)
		    {

		    if (f[c] < f[c - weight[k]] + value[k])
			{
			f[c] = f[c - weight[k]] + value[k];
			M[capacity * k + c] = 1;
			}
		    }
		}

	    chrono.stopChrono();
	    time_chrono += chrono.getElapsedChrono();
	    time_omp += chrono.getElapsedOmpTime();

#if PRINT
	    printResults(capacity, sumWeight, numelem[i], f, M, weight,
		    value, chrono.getElapsedChrono());
#endif
	    free(M);
	    free(f);
	    free(value);
	    free(weight);

	    }

	time_chrono = time_chrono / (double) NCYCLES;
	time_omp = time_omp / (double) NCYCLES;

	cout << "# of elements: " << numelem[i] << endl;
	cout << "Average time[ms]: " << time_chrono << endl;
	cout << "Omp time[s]: " << time_omp << endl;

	out_file << "# of elements: " << numelem[i] << endl;
	out_file << "Average time[ms]: " << time_chrono << endl;
	out_file << "Omp time[s]: " << time_omp << endl;

	}
    echrono.stopChrono();

    cout << "Execution-chrono time[ms]: " << echrono.getElapsedChrono() << endl;
    cout << "Execution-Omp time: " << echrono.getElapsedOmpTime() << endl;
    cout << "Execution-time : " << echrono.getElapsedTime() << endl;
    out_file << "Execution-chrono time[ms]: " << echrono.getElapsedChrono()
	    << endl;
    out_file << "Execution-Omp time: " << echrono.getElapsedOmpTime() << endl;
    out_file << "Execution-time : " << echrono.getElapsedTime() << endl;

    out_file.close();
    return 0;
    }

void printResults(int capacity, int sumWeight, int numelem, int* f, int* M,
	int* weight, int* value, long runtime)
    {

    cout << "\nPRINTOUT OF THE RESULTS: " << endl;
    cout << "*************************************************" << endl;

#if VERBOSE    
    cout << "Matrix of decisions M[items][capacity]: " << endl;
    for (int i = 0; i < numelem * (capacity+1); i++)
	{
	cout << *(M + i) << "; ";
	if ((i + 1) % (capacity+1) == 0)
	    {
	    cout << endl;
	    }
	}
#endif    

    cout << "SumWeight: " << sumWeight << "\t Capacity: " << capacity << "\n";
    cout << "Knapsack's worth: " << f[capacity] << endl;

#if VERBOSE
    cout << "Chosen items are:" << endl;
#endif

    int cap = capacity;
    int capacita = 0;
    for (int item = numelem - 1; item > -1; item--)
	{
	for (int c = cap - 1; c > -1; c--)
	    {
	    if (M[item * (capacity) + c] != 0)
		{
#if VERBOSE
		cout << "Value " << item << ": " << value[item];
#endif
		capacita += weight[item];
		cap = cap - weight[item];
		break;
		}
	    else if (M[item * (capacity) + c] == 0)
		{
		c = 0;
		}
	    }

	}

    cout << "Weight of the Knapsack: " << capacita << endl;
    cout << " profiling time: " << runtime * 1L << "ms" << endl;
    cout << "*************************************************" << endl;
    cout << "END OF THE PRINTOUT" << endl;

    }

