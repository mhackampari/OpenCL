/* 
 * File:   Chrono.cpp
 * Author: Nico
 * 
 * Created on 13 aprile 2015, 13.41
 */

#include "Chrono.h"

Chrono::Chrono() {
    resetted = true;
    running = false;
}

Chrono::~Chrono() {
}

void Chrono::startChrono() {
    running=true;
    beg = std::chrono::high_resolution_clock::now();
}

void Chrono::stopChrono() {
    if (running) {
        end = std::chrono::high_resolution_clock::now();
        running = false;
    }
}

void Chrono::resetChrono() {
    //bool wereRunning = running;
    if (isRunningChrono())
        stopChrono();
    resetted = true;
}

bool Chrono::isRunningChrono() {
    return running;
}

//results are in ms

unsigned long Chrono::getTimeChrono() {
    if (running) {
        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
        return elapsed_time.count();
    } else {
        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
        return elapsed_time.count();
    }
}
