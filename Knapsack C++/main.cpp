/* 
 * File:   main.cpp
 * Author: terminator
 *
 * Created on July 22, 2015, 7:08 PM
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <memory.h>

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

        default_random_engine random_engine;
        uniform_int_distribution<int> unifdist(1, numelem);

        for (int i = 0; i < numelem; i++) {

            weight[i] = rand() % numelem + 1; //unifdist(random_engine); 
            sum += weight[i];
            value[i] = weight[i] + rand() % numelem + 1; //+ 50;
            //cout << i << "\t" << "weight: " << weight[i] << "\t" << "value: " << value[i] << endl;

        }
        capacity = sum / 2;
        cout << "Capacity: " << capacity << endl;
    }

    ~TestData() {
        cout << "\nTEST DATA HAVE BEEN JUST DESTROYED\n";
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
    chrono::high_resolution_clock::time_point beg;
    chrono::high_resolution_clock::time_point end;
    chrono::duration<int, std::milli> elapsed_time;

public:

    Chrono() {
        resetted = true;
        running = false;
    }

    ~Chrono() {
    }

    void startChrono() {
        running = true;
        beg = std::chrono::high_resolution_clock::now();
    }

    void stopChrono() {
        if (running) {
            end = std::chrono::high_resolution_clock::now();
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

    unsigned long getTimeChrono() {
        if (running) {
            elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
            return elapsed_time.count();
        } else {
            elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
            return elapsed_time.count();
        }
    }
};

void printResults(int capacity, int sumWeight, int numelem, int* f, int* M, int* weight, int* value, long runtime);

int main(int argc, char** argv) {

    int numelem[] = {5, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    Chrono c;

    for (int i = 0; i < sizeof (numelem) / sizeof (int) - 8; i++) {
        TestData *t = new TestData(numelem[i]);
        int sumWeight = t->getSum();
        int sumK = sumWeight;
        int capacity = t->getCapacity();
        int *value = (int *) calloc(numelem[i], sizeof (int));
        int *weight = (int *) calloc(numelem[i], sizeof (int));
        memcpy(value, t->getValue(), sizeof (int)*numelem[i]);
        memcpy(weight, t->getWeight(), sizeof (int)*numelem[i]);
        delete t;
        int *f = (int *) calloc((capacity + 1), sizeof (int)); //f[0, ..., capacity]
        //segmentation fault can be caused because of max memory limit(ram)
        int *M = (int *) calloc(numelem[i] * (capacity + 1), sizeof (int)); //M[numelem][capacity]
        if (M == NULL) cout << "NOT ENOUGH AVAILABLE SPACE!!!! TRY WITH LESS ELEMENTS\n";
        
        c.startChrono();
        for (int k = 0; k < numelem[i]; k++) {

            sumK = sumK - weight[k];
            int cmax = 0;
            cmax = max(capacity - sumK, weight[k]);

            for (int c = capacity; c >= cmax; c--) {

                if (f[c] < f[c - weight[k]] + value[k]) {
                    f[c] = f[c - weight[k]] + value[k];
                    M[capacity * k + c] = 1;
                }
            }

        }
        c.stopChrono();
        printResults(capacity, sumWeight, numelem[i], f, M, weight, value, c.getTimeChrono());
        delete M, f, value, weight;
    }

    return 0;
}

void printResults(int capacity, int sumWeight, int numelem, int* f, int* M, int* weight, int* value, long runtime) {

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
    cout << " profiling time: " << runtime*1L << "ms" <<endl;
    cout << "*************************************************" << endl;
    cout << "END OF THE PRINTOUT" << endl;

}

