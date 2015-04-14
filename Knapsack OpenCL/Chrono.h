/* 
 * File:   Chrono.h
 * Author: Nico
 *
 * Created on 13 aprile 2015, 13.41
 */

#ifndef CHRONO_H
#define	CHRONO_H
#include <chrono>
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
    unsigned long getTimeChrono();
private:
    bool resetted;
    bool running;
    std::chrono::high_resolution_clock::time_point beg;
    std::chrono::high_resolution_clock::time_point end;
    std::chrono::duration<int, std::milli> elapsed_time;
};

#endif	/* CHRONO_H */

