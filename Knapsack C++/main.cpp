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
            cout << i << "weight: " << weight[i] << "\t" << "value: " << value[i] << endl;
            capacity = sum / 2;
            cout << "Capacity: " << capacity << endl;

        }
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

void printResults(int capacity, int sumWeight, int numelem, int* f1, int* f0, int* M, int* weight, long runtime);

int main(int argc, char** argv) {
    int numelem[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int capacity;
    

    return 0;
}

printResults(int capacity, int sumWeight, int numelem, int* f1, int* f0, int* M, int* weight, long runtime) {

    cout << "\nPRINTOUT OF THE RESULTS: " << endl;
    cout << "*************************************************" << endl;

    /*
         cout << "Evolution of the Knapsack worth F[x]: ";
         //*logfile << "Evolution of the Knapsack worth F[x]: ";
        for (int i = 0; i < capacity + 1; i++) {
            if (numelem % 2 == 0) {
                cout << f0[i] << " ";
     *logfile << f0[i] << " ";
            } else {
                cout << *(f1 + i) << " ";
     *logfile << *(f1 + i) << " ";
            }

        }

        cout << endl;
     * */
    cout << "Matrix of decisions M[items][capacity]: " << endl;
    for (int i = 0; i < numelem * capacity; i++) {
        cout << *(M + i) << "; ";
        if ((i + 1) % (capacity) == 0) {
            cout << endl;
        }
    }

    cout << "SumWeight: " << sumWeight << "\t Capacity: " << capacity << "\n";
    cout << "Knapsack's worth: ";
    if (numelem % 2 == 0) {
        cout << f0[capacity] << endl;
    } else {
        cout << f1[capacity] << endl;
    }

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
    memset(f0, 0, sizeof (capacity)*(capacity + 1));
    memset(f1, 0, sizeof (capacity)*(capacity + 1));

    cout << "Weight of the Knapsack: " << capacita << endl;
    cout << " profiling time: " << runtime << endl;
    cout << "*************************************************" << endl;
    cout << "END OF THE PRINTOUT" << endl;

}

