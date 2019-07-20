#ifndef HOST__TIMER_H__INCLUDED
#define HOST__TIMER_H__INCLUDED

#include <cstdint>

class Timer {
public:

    Timer() {}
    virtual ~Timer() {}

    virtual uint32_t getElapsedMillis() = 0;
    virtual void wait(uint32_t millis) = 0;

private:

    Timer(const Timer&);
    Timer& operator=(const Timer&);
};

#endif
