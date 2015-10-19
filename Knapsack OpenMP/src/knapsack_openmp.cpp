/*
 * File:   main.cpp
 * Author: terminator
 *
 * Created on July 22, 2015, 7:08 PM
 * https://computing.llnl.gov/tutorials/openMP/
 */

#include <cstdlib>
#include <iostream>
#include <memory.h>
#include <fstream>
#include <math.h>
#include "Chrono.h"
#include "TestData.h"

using namespace std;
#ifndef PRINT
#define PRINT true
#endif

#ifndef VERBOSE
#define VERBOSE false
#endif

#ifndef SUPERVERBOSE
#define SUPERVERBOSE false
#endif

#ifndef NCYCLES
#define NCYCLES 1
#endif

using namespace std;

void printResults(unsigned int capacity, unsigned int sumWeight, unsigned int numelem, unsigned int* f, unsigned int* M,
	unsigned int* weight, unsigned int* value, long runtime)
    {

    cout << "\nPRINTOUT OF THE RESULTS: " << endl;
    cout << "*************************************************" << endl;

    int rows = ceil(numelem / 32.0);
    int cols = capacity + 1;

#if SUPERVERBOSE
    for (int i = 0; i < rows * cols; i++)
	{

	unsigned int x = ceil(log2(M[i])); //x is the position 0...31
	cout << i << ": # of bits: " << x << "\t M[]: " << M[i] << "\n";
	if ((i + 1) % cols == 0)
	    {
	    cout << "***" << endl;
	    }
	}
#endif

    int c = capacity;
    int bit32 = 32;
    int worth = 0;
    int capacita = 0;

    cout << "Chosen items are:" << endl;
    for (int i = ceil(numelem / 32.0) - 1; i >= 0; i--)
	{
	bit32 = 32;
	//cout << "**numelem / 32.0**" << endl;
	while (bit32 > 0 && c > 0)
	    {

	    unsigned int m = M[i * (capacity+1) + c];
	    unsigned int x = ceil(log2(M[i * (capacity+1) + c])); // gives the position of the msb: 000100 = 2

	    unsigned int bit32pw = pow(2, (bit32 - 1));
	    if (bit32pw == (bit32pw & m))
		{ //binary: 1000 == 1000 <- (1000 & 1010)
#if VERBOSE
		cout << "M[" << i * (capacity+1) + c << "]: "
			<< M[i * (capacity+1) + c] << endl;
		cout << x << "; ";
		cout << i * 32 + bit32 - 1 << " \tvalue: "
			<< value[i * 32 + bit32 - 1] << "\tweight: "
			<< weight[i * 32 + bit32 - 1] << endl;
#endif
		c -= weight[i * 32 + bit32 - 1];
		capacita += weight[i * 32 + bit32 - 1];
		worth += value[i * 32 + bit32 - 1];
		}
	    bit32--;

	    }
	}

    cout << "Knapsack f: " << f[capacity] << endl;
    cout << "SumWeight: " << sumWeight << "\t Capacity: " << capacity << endl;
    cout << "Knapsack's worth: " << worth
	    << "\t Weight of the selected objects: " << capacita << endl;
    cout << " profiling time: " << runtime * 1L << "ms" << endl;
    cout << "*************************************************" << endl;
    cout << "END OF THE PRINTOUT" << endl;

    }

void init(unsigned int &capacity, unsigned int &sumWeight, unsigned int numelem, unsigned int **f0, unsigned int **f1,
	unsigned int **M, unsigned int **weight, unsigned int **value)
    {

    TestData t(numelem, VERBOSE);
    sumWeight = t.getSum();
    capacity = t.getCapacity();
    *value = (unsigned int *) calloc(numelem, sizeof(unsigned int));
    *weight = (unsigned int *) calloc(numelem, sizeof(unsigned int));
    memcpy(*value, t.getValue(), sizeof(unsigned int) * numelem);
    memcpy(*weight, t.getWeight(), sizeof(unsigned int) * numelem);
    *f0 = (unsigned int *) calloc((capacity + 1), sizeof(unsigned int)); //f[0, ..., capacity]
    *f1 = (unsigned int *) calloc((capacity + 1), sizeof(unsigned int));
    //segmentation fault can be caused because of max memory limit(ram)
    unsigned int rows = ceil(numelem / (sizeof(unsigned int) * 8.0));
    *M = (unsigned int *) calloc(
	    rows * (capacity + 1),
	    sizeof(unsigned int)); //M[numelem][capacity]
    if (M == NULL)
	cout << "NOT ENOUGH AVAILABLE SPACE!!!! TRY WITH LESS ELEMENTS\n";


    }

int main(int argc, char** argv)
    {
    fstream out_file("knapsack_results.txt", ios::out);
    unsigned int numelem[] =
	{
	1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000
	};
    unsigned int nelements = sizeof(numelem) / sizeof(unsigned int);
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

	    unsigned int sumWeight = 100;
	    unsigned int capacity = sumWeight / 50;
	    unsigned int *value, *weight, *f0, *f1, *M;

	    init(capacity, sumWeight, numelem[i], &f0, &f1, &M, &weight,
		    &value);

	    unsigned int sumK = sumWeight;
	    unsigned int row = 0;
	    unsigned int power = 0;

	    chrono.startChrono();

	    for (unsigned int k = 0; k < numelem[i]; k++)
		{

		sumK = sumK - weight[k];
		unsigned int cmax = 0;
		cmax = max((int)(capacity - sumK), (int)weight[k]);
		unsigned int c = capacity;
		power = k % 32;
		if (k % 32 == 0){
		    row += 1;
		}

		if (k % 2 == 0)
		    {

#pragma omp parallel for
		    for (c = capacity; c >= cmax; c--)
			{

			//FALSE SHARING

			if (f0[c] < f0[c - weight[k]] + value[k])
			    {
			    f1[c] = f0[c - weight[k]] + value[k];
			    M[(capacity+1) * (row-1) + c] += pow(2, power);


			    }
			else
			    {
			    f1[c] = f0[c];

			    }
			}
		    }

		else
		    {

#pragma omp parallel for
		    for (c = capacity; c >= cmax; c--)
			{

			//FALSE SHARING

			if (f1[c] < f1[c - weight[k]] + value[k])
			    {
			    f0[c] = f1[c - weight[k]] + value[k];
			    M[(capacity+1) * (row-1) + c] += pow(2, power);

			    }
			else
			    {
			    f0[c] = f1[c];
			    }
			}

		    }

		}

	    chrono.stopChrono();
	    time_chrono += chrono.getElapsedChrono();
	    time_omp += chrono.getElapsedOmpTime();

#if PRINT
	    if (numelem[i] % 2 == 0)
	    printResults(capacity, sumWeight, numelem[i], f0, M,
		    weight, value, chrono.getElapsedChrono());
	    else
	    printResults(capacity, sumWeight, numelem[i], f1, M,
		    weight, value, chrono.getElapsedChrono());
#endif
	    free(M);
	    free(f0);
	    free(f1);
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
	cout << "#######################################################\n";

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

