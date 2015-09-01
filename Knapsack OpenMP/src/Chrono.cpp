/* 
 * File:   Chrono.cpp
 * Author: Nico
 * 
 * Created on 13 aprile 2015, 13.41
 */

#include "Chrono.h"


Chrono::Chrono()
	{
	resetted = true;
	running = false;
    start_omp = 0;
    end_omp = 0;
    elapsed_omp_time = 0;
    start_time = 0;
    end_time = 0;
    elapsed_time = 0;
	}

Chrono::~Chrono()
	{
		cout << "Chrono destroyed\n";
	}

void Chrono :: startChrono()
	{
	running = true;
	start_chrono = std::chrono::high_resolution_clock::now();
	start_omp = omp_get_wtime();
	time(&start_time);
	}

void Chrono :: stopChrono()
	{
	if (running)
	    {
	    end_chrono = std::chrono::high_resolution_clock::now();
	    end_omp = omp_get_wtime();
	    time(&end_time);
	    running = false;
	    }
	}

void Chrono :: resetChrono()
	{
	if (isRunningChrono())
	    stopChrono();
	resetted = true;
	}

bool Chrono :: isRunningChrono()
	{
	return running;
	}

unsigned long Chrono :: getElapsedChrono()
	{
	if (running)
	    {
	    stopChrono();
	    }
	elapsed_chrono_time = std::chrono::duration_cast<
		std::chrono::milliseconds>(end_chrono - start_chrono);
	return elapsed_chrono_time.count();
	}

double Chrono :: getElapsedOmpTime()
	{
	if (running)
	    {
	    stopChrono();
	    }
	elapsed_omp_time = end_omp - start_omp;
	return elapsed_omp_time;
	}

    double Chrono :: getElapsedTime()
	{
	if (running)
	    {
	    stopChrono();
	    }
	elapsed_time = end_time - start_time;
	return elapsed_time;
	}

    void Chrono :: printChrono()
	{
	if (running)
	    {
	    stopChrono();
	    }

	cout << "Chrono time[ms]: " << elapsed_chrono_time.count() << endl;
	cout << "Omp time[s]: " << elapsed_time << endl;
	cout << "<time.h> time[s]: " << elapsed_time << endl;
	}

    void Chrono :: printChronoToFile(fstream &file)
	{
	if (running)
	    {
	    stopChrono();
	    }

	file << "Chrono time[ms]: " << elapsed_chrono_time.count() << endl;
	file << "Omp time[s]: " << elapsed_time << endl;
	file << "<time.h> time[s]: " << elapsed_time << endl;
	}

