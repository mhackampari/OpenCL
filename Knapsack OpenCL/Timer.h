/* 
 * File:   Timer.h
 * Author: Nico
 *
 * Created on 12 aprile 2015, 9.18
 */

#ifndef TIMER_H
#define	TIMER_H
#endif

#include <time.h>
class Timer {
public:
    Timer();
    Timer(const Timer& orig);
    virtual ~Timer();
    void start();
    void stop();
    void reset();
    bool isRunning();
    unsigned long getTime();
    bool isOver(unsigned long seconds);
private:
    bool resetted;
    bool running;
    unsigned long beg;
    unsigned long end;
};


