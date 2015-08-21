/* 
 * File:   Chrono.h
 * Author: Nico
 *
 * Created on 13 aprile 2015, 13.41
 */

#ifndef CHRONO_H
#define	CHRONO_H
#include <chrono>
#include <iostream>
#include <omp.h>
#include <omp.h>
#include <fstream>
 using namespace std;
//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm#duration
//http://stackoverflow.com/questions/13666320/several-questions-about-chrono-header-in-c-11
class Chrono {

public:
    Chrono();
    ~Chrono();
    void startChrono();
    void stopChrono();
    void resetChrono();
    bool isRunningChrono();
    unsigned long getElapsedChrono();
    double getElapsedOmpTime();
    double getElapsedTime();
    void printChrono();
    void printChronoToFile(fstream &file);
private:
    bool resetted;
    bool running;
    chrono::high_resolution_clock::time_point start_chrono;
    chrono::high_resolution_clock::time_point end_chrono;
    chrono::duration<int, std::milli> elapsed_chrono_time;
    double start_omp, end_omp, elapsed_omp_time;
    long start_time, end_time, elapsed_time;
};

#endif	/* CHRONO_H */

