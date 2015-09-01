/*
 * File:   main.cpp
 * Author: terminator
 *
 * Created on July 22, 2015, 7:08 PM
 * https://computing.llnl.gov/tutorials/openMP/
 */
/*
#include <cstdlib>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <memory.h>
#include <fstream>
#include <omp.h>
#include <thread>
#include <time.h>

using namespace std;

class TestData {
private:
	vector<int> weight;
	vector<int> value;
	int sum;
	int capacity;
	int numelem;

public:

	TestData() {

		numelem = 1000;
		weight.resize(numelem, 0);
		value.resize(numelem, 0);
		sum = 0;
		//picked this interval without any underlying criteria
		default_random_engine random_engine;
		uniform_int_distribution<int> unifdist(1, numelem);

		for (int i = 0; i < numelem; i++) {

			weight[i] = unifdist(random_engine);
			sum += weight[i];
			value[i] = weight[i] + 50;

		}

		capacity = sum / 2;

	}

	TestData(int numel) {

		numelem = numel;
		weight.resize(numelem, 0);
		value.resize(numelem, 0);
		sum = 0;

		for (int i = 0; i < numelem; i++) {

			weight[i] = rand() % numelem + 1; //unifdist(random_engine);
			sum += weight[i];
			value[i] = weight[i] + rand() % numelem + 1; //+ 50;
			//cout << i << "\t" << "weight: " << weight[i] << "\t" << "value: " << value[i] << endl;

		}
		capacity = sum / 2;
		//cout << "Capacity: " << capacity << endl;
	}

	~TestData() {

	}

	int* getValue() {
		return value.data();
	}

	int* getWeight() {
		return weight.data();
	}

	int getSum() {
		return sum;
	}

	int getCapacity() {
		return capacity;
	}

	int getNumelem() {
		return numelem;
	}
};

class Chrono {
	//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm#duration
	//http://stackoverflow.com/questions/13666320/several-questions-about-chrono-header-in-c-11
private:
	bool resetted;
	bool running;
	chrono::high_resolution_clock::time_point start_chrono;
	chrono::high_resolution_clock::time_point end_chrono;
	chrono::duration<int, std::milli> elapsed_chrono_time;
	double start_omp, end_omp, elapsed_omp_time;
	long start_time, end_time, elapsed_time;

public:

	Chrono() {
		resetted = true;
		running = false;
	}

	~Chrono() {
	}

	void startChrono() {
		running = true;
		start_chrono = std::chrono::high_resolution_clock::now();
		start_omp = omp_get_wtime();
		time(&start_time);

	}

	void stopChrono() {
		if (running) {
			end_chrono = std::chrono::high_resolution_clock::now();
			end_omp = omp_get_wtime();
			time(&end_time);
			running = false;
		}
	}

	void resetChrono() {
		if (isRunningChrono())
			stopChrono();
		resetted = true;
	}

	bool isRunningChrono() {
		return running;
	}

	unsigned long getElapsedChrono() {
		if (running) {
			stopChrono();
		}
		elapsed_chrono_time = std::chrono::duration_cast
				< std::chrono::milliseconds > (end_chrono - start_chrono);
		return elapsed_chrono_time.count();
	}
	double getElapsedOmpTime() {
		if (running) {
			stopChrono();
		}
		elapsed_omp_time = end_omp - start_omp;
		return elapsed_omp_time;

	}
	long getElapsedTime() {
		if (running) {
			stopChrono();
		}
		elapsed_time = end_time - start_time;
		return elapsed_time;
	}
	void printElapsedTime() {
		if(running){
			stopChrono();
		}

		cout << "Chrono time[ms]: " << elapsed_chrono_time.count() << endl;
		cout << "Omp time[s]: " << elapsed_time << endl;
		cout << "<time.h> time[s]: " << elapsed_time << endl;
	}

	void printElapsedTimeToFile(fstream &file) {
			if(running){
				stopChrono();
			}

			file << "Chrono time[ms]: " << elapsed_chrono_time.count() << endl;
			file << "Omp time[s]: " << elapsed_time << endl;
			file << "<time.h> time[s]: " << elapsed_time << endl;
		}

};

void printResults(int capacity, int sumWeight, int numelem, int* f, int* M,
		int* weight, int* value, long runtime);
void chronoVSomptime();

int main(int argc, char** argv) {
	Chrono chrono, echrono; //use chronoVSomptime() to see difference
	echrono.startChrono();
	fstream output_file;
	output_file.open("results_knapsack_omp", ios_base::out);
	double timeomp;
	long total_time= 0;
	long total_chrono = 0;
	bool print = false;

	int numelem[] = { 5, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
	unsigned int nelements = sizeof(numelem) / sizeof(int);
	const unsigned int ncycles = 100;

	cout << "NUM MAX THREADS: " << omp_get_max_threads() << endl;
	omp_set_num_threads(omp_get_max_threads());

	for (unsigned int i = 0; i < nelements; i++) {
		total_time = 0;
		timeomp = 0;
		total_chrono = 0;
		cout << "ROUND: " << i << endl;
		output_file << "ROUND: " << i << endl;

		for (unsigned int j = 0; j < ncycles; j++) {

			TestData *t = new TestData(numelem[i]);
			int sumWeight = t->getSum();
			if (j == 0) {
				cout << "sumweight: " << sumWeight << endl;
				output_file << "sumweight: " << sumWeight << endl;
			}
			int sumK = sumWeight;
			int capacity = t->getCapacity();
			int *value = (int *) calloc(numelem[i], sizeof(int));
			int *weight = (int *) calloc(numelem[i], sizeof(int));
			memcpy(value, t->getValue(), sizeof(int) * numelem[i]);
			memcpy(weight, t->getWeight(), sizeof(int) * numelem[i]);
			delete t;
			int *f1 = (int *) calloc((capacity + 1), sizeof(int)); //f[0, ..., capacity]
			int *f0 = (int *) calloc((capacity + 1), sizeof(int));
			//segmentation fault can be caused because of max memory limit(ram)
			int *M = (int *) calloc(numelem[i] * (capacity + 1), sizeof(int)); //M[numelem][capacity]
			if (M == NULL)
				cout
						<< "NOT ENOUGH AVAILABLE SPACE!!!! TRY WITH LESS ELEMENTS\n";

			chrono.startChrono();
			for (int k = 0; k < numelem[i]; k++) {

				sumK = sumK - weight[k];
				int cmax = 0;
				cmax = max(capacity - sumK, weight[k]);
				int c = capacity;

				if (k % 2 == 0) {
#pragma omp parallel for
					for (c = capacity; c >= cmax; c--) {
						//RACE CONDITION

						if (f0[c] < f0[c - weight[k]] + value[k]) {
							f1[c] = f0[c - weight[k]] + value[k];
							M[capacity * k + c] = 1;
						} else {
							f1[c] = f0[c];
						}
					}
				} else {
#pragma omp parallel for
					for (c = capacity; c >= cmax; c--) {

						//RACE CONDITION

						if (f1[c] < f1[c - weight[k]] + value[k]) {
							f0[c] = f1[c - weight[k]] + value[k];
							M[capacity * k + c] = 1;
						} else {
							f0[c] = f1[c];
						}
					}

				}

				if (print) {
					if (numelem[i] % 2 == 0)
						printResults(capacity, sumWeight, numelem[i], f0, M,
								weight, value, chrono.getElapsedChrono());
					else
						printResults(capacity, sumWeight, numelem[i], f1, M,
								weight, value, chrono.getElapsedChrono());
				}

			}

			chrono.stopChrono();

			total_chrono += chrono.getElapsedChrono();
			timeomp += chrono.getElapsedOmpTime();
			total_time += chrono.getElapsedTime();

			free(M);
			free(f0);
			free(f1);
			free(value);
			free(weight);

		}

		total_time = total_time / (double) ncycles;
		timeomp = timeomp / (double) ncycles;
		total_chrono = total_chrono / (double) ncycles;

		cout << "# of elements: " << numelem[i] << endl;
		cout << "Chrono time[ms]: " << total_chrono << endl;
		cout << "Omp time[s]: " << timeomp << endl;
		cout << "Total time: " << total_time << endl;

		output_file << "# of elements: " << numelem[i] << endl;
		output_file << "Chrono time[ms]: " << total_chrono << endl;
		output_file << "Omp time[s]: " << timeomp << endl;
	}
	echrono.stopChrono();

	cout << "EX-chrono time[ms]: " << echrono.getElapsedChrono()<< endl;
	cout << "Ex-Omp time: " << echrono.getElapsedOmpTime() << endl;
	cout << "Ex-time : " << echrono.getElapsedTime() << endl;
	output_file.close();
	return 0;
}

void printResults(int capacity, int sumWeight, int numelem, int* f, int* M,
		int* weight, int* value, long runtime) {

	cout << "\nPRINTOUT OF THE RESULTS: " << endl;
	cout << "*************************************************" << endl;
	/*
	 cout << "Matrix of decisions M[items][capacity]: " << endl;
	 for (int i = 0; i < numelem * (capacity+1); i++) {
	 cout << *(M + i) << "; ";
	 if ((i + 1) % (capacity+1) == 0) {
	 cout << endl;
	 }
	 }
	 */

	cout << "SumWeight: " << sumWeight << "\t Capacity: " << capacity << "\n";

	cout << "Knapsack's worth: " << f[capacity] << endl;

	//cout << "Chosen items are:" << endl;
	int cap = capacity;
	int capacita = 0;
	for (int item = numelem - 1; item > -1; item--) {
		for (int c = cap - 1; c > -1; c--) {
			if (M[item * (capacity) + c] != 0) {
				//cout << "Value " << item << ": " << value[item];
				capacita += weight[item];
				cap = cap - weight[item];
				break;
			} else if (M[item * (capacity) + c] == 0) {
				c = 0;
			}
		}

	}

	cout << "Weight of the Knapsack: " << capacita << endl;
	cout << " profiling time: " << runtime * 1L << "ms" << endl;
	cout << "*************************************************" << endl;
	cout << endl;

}

void chronoVSomptime() {
	Chrono chrono;
	double timeomp, tomp1, tomp2;
	time_t t1, t2;

	chrono.startChrono();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	chrono.stopChrono();
	cout << "chrono MS: " << chrono.getElapsedChrono()<< endl;

	chrono.startChrono();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	chrono.stopChrono();
	cout << "chrono MS: " << chrono.getElapsedChrono()<< endl;
	cout << "tomp in Sec: " << chrono.getElapsedOmpTime() << endl;
	cout << "Time in Sec " << chrono.getElapsedTime() << endl;

	tomp1 = omp_get_wtime();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	tomp2 = omp_get_wtime();
	timeomp = tomp2 - tomp1;
	cout << "tomp in Sec: " << timeomp << endl;

	time(&t1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	time(&t2);
	cout << "Time in Sec " << difftime(t2, t1) << endl;
	exit(0);
}

*/
