/* 
 * File:   Timer.cpp
 * Author: Nico
 * 
 * Created on 12 aprile 2015, 9.18
 */

#include "Timer.h"
//http://stackoverflow.com/questions/15235218/c-timer-milliseconds
//better not to use timer but chrono
Timer::Timer() {
    resetted = true;
	running = false;
	beg = 0;
	end = 0;
}

Timer::~Timer() {
}

void Timer::start() {
	if(! running) {
		if(resetted)
			beg = clock();
		else
			beg -= end - clock();
		running = true;
		resetted = false;
	}
}

void Timer::stop() {
	if(running) {
		end = clock();
		running = false;
	}
}

void Timer::reset() {
	//bool wereRunning = running;
	if(isRunning())
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	
}

bool Timer::isRunning() {
	return running;
}

//results are ms
float Timer::getTime() {
	if(running)
		return 1000.0*(float)(clock() - beg) / CLOCKS_PER_SEC;
	else
		return 1000.0*(float)(end - beg)/CLOCKS_PER_SEC;
}

bool Timer::isOver(unsigned long seconds) {
	return seconds >= getTime();
}
